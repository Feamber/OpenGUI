#include "OpenGUI/Configure.h"
#include "OpenGUI/Style2/generated/effects.h"
#include "Yoga.h"
#include <algorithm>
#include <cstdio>
#include <vector>
#include <memory>
#include "OpenGUI/Core/nanovg.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/KeyEvent.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/VisualWindow.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/IOThread.h"
#include "OpenGUI/Managers/RenderTextureManager.h"
#include "OpenGUI/XmlParser/BaseXmlFactory.h"
#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/StdLog.h"
#include "OpenGUI/Bind/Bind.h"
#include "Text/godot/text_server_adv.h"
#include "OpenGUI/Style2/generated/position.h"
#include <queue>

OGUI::WindowContext::WindowContext(WindowHandle window)
	: window(window)
{
	ui = new VisualWindow();
	ui->handle = window;
};

OGUI::WindowContext::~WindowContext()
{
	if(ui)
		VisualElement::DestoryTree(ui);
};

godot::TextServer* OGUI::Context::GetTextServer()
{
	if(!_textServer)
	{
		_textServer = std::make_unique<godot::TextServerAdvanced>();
	}
	return _textServer.get();
}

template <class T>
class backwards {
    T& _obj;
public:
    backwards(T &obj) : _obj(obj) {}
    auto begin() {return _obj.rbegin();}
    auto end() {return _obj.rend();}
};

namespace OGUI
{
	void TransformRec(VisualElement* element, bool dirty = false)
	{
		dirty |= element->_transformDirty;
		if(dirty)
			element->UpdateWorldTransform();
		element->Traverse([&](VisualElement* next) { TransformRec(next, dirty); });
	}

	VisualElement* PickRecursive(VisualElement* element, Vector2f point)
	{
		if(!element->Visible() || !element->IsPick())
			return nullptr;
		std::vector<VisualElement*> children;
		element->GetChildren(children);
		bool clipping = element->IsClippingChildren();
		bool intersection = false;
		if(clipping)
		{
			intersection = element->Intersect(point);
			if(!intersection)
				return nullptr;
		}
		for (auto child : backwards(children))
			if(auto picked = PickRecursive(child, point))
				return picked;

		//std::cout << "OnMouseDown: " << localPoint.X << "," << localPoint.Y << std::endl;
		//std::cout << "Name: " << element->_name << std::endl;
		//std::cout << "Rect: " << element->GetRect().min.X << element->GetRect().min.Y << std::endl;
		if (intersection || element->Intersect(point))
		{
			if (element->_isPseudoElement)
				return element->GetHierachyParent();
			else
				return element;
		}
		
		else return nullptr;
	}
	void CacheLayoutRec(VisualElement* element)
	{
		element->_prevLayout = element->GetLayout();
		element->Traverse([&](VisualElement* next) { CacheLayoutRec(next); });
	}
	void CheckLayoutRec(VisualElement* element)
	{
		if (element->_prevLayout != element->GetLayout())
			element->MarkTransformDirty();
		else
			element->Traverse([&](VisualElement* next) { CheckLayoutRec(next); });
	}
	void UpdateLayout(WindowContext& wctx, VisualElement* element)
	{
		float width = wctx.GetWidth(), height = wctx.GetHeight();
		auto& ctx = Context::Get();
		
		if(!wctx._layoutDirty)
			return;
		CacheLayoutRec(element);
		while (wctx._layoutDirty)
		{
			element->CalculateLayout(width, height);
			wctx._layoutDirty = false;
		}
		CheckLayoutRec(element);
	}
	void UpdateScrollSize(VisualElement* element)
	{
		element->Traverse([&](VisualElement* next) 
		{ 
			next->UpdateScrollSize(); 
			UpdateScrollSize(next); 
		});
	}
	void UpdateVisibilityRec(VisualElement* element, std::vector<Matrix4x4>& clipStack)
	{
		if(!clipStack.empty())
			element->_clipped = element->CheckClip(clipStack.back());
		bool clippingChild = element->IsClippingChildren();
		if(clippingChild)
			clipStack.push_back(element->ApplyClipping());
		element->Traverse([&](VisualElement* next)
		{
			UpdateVisibilityRec(next, clipStack);
		});
		if(clippingChild)
			clipStack.pop_back();
	}
	void UpdateVisibility(VisualElement* element)
	{
		std::vector<Matrix4x4> clipStack;
		UpdateVisibilityRec(element, clipStack);
	}
}

OGUI::WindowContext& OGUI::Context::Create(const OGUI::WindowHandle window)
{
	return GetOrRegisterWindowContext(window);
}

void OGUI::Context::Remove(const OGUI::WindowHandle window)
{
	for(int i = 0; i < windowContexts.size(); ++i)
	{
		auto&& ctx = windowContexts[i];
		if(ctx->window == window)
		{
			if(_currentActivateWindow == ctx->GetWindowUI())
			{
				_currentActivateWindow = nullptr;
				_keyboardFocused = nullptr;
			}
			auto temp = std::move(ctx);
			windowContexts.erase(windowContexts.begin() + i);
			return;
		}
	}
}

void OGUI::Context::MarkLayoutDirty(VisualElement* element)
{
	if(element->GetRoot()->GetTypeName() == "VisualWindow")
	{
		auto& wctx = GetWindowContext(((VisualWindow*)element->GetRoot())->handle);
		wctx._layoutDirty = true;
	}
}

void OGUI::Context::InvalidateCssCache()
{
	for(auto& window : windowContexts)
	{
		window->_cssCacheInvalidated = true;
		window->_layoutDirty = true;
	}
}
 
void OGUI::Context::Update(const OGUI::WindowHandle window, float dt)
{
	auto& wctx = GetOrRegisterWindowContext(window);
	auto root = wctx.GetWindowUI();
	// Texture Streaming
	textureManager->Update();
	_deltaTime = dt;
	styleSystem.Update(root, wctx._cssCacheInvalidated);
	wctx._cssCacheInvalidated = false;
	UpdateLayout(wctx, root);
	UpdateScrollSize(root);
	TransformRec(root);
	UpdateVisibility(root);
	std::deque<VisualElement*> queue;
	auto& ctx = *wctx.currentDrawCtx;
	queue.push_back(root);
	while(!queue.empty())
	{
		auto next = queue.front();
		queue.pop_front();
		next->Update(dt);
		next->Traverse([&](VisualElement* child) {queue.push_back(child);});
	}
}

namespace OGUI
{
	constexpr size_t maxClipNum = 2;
	struct DrawElement
	{
		int zorder;
		VisualElement* element;
		Matrix4x4 clip[maxClipNum];
		int clipNum = 0;
		uint32_t drawPass;
		bool operator<(const DrawElement& other) const
		{
			return zorder < other.zorder;
		}
	};
	struct DrawList
	{
		RenderTargetViewHandle renderTarget = nullptr;
		std::vector<DrawElement> list;
		float4x4 inverseTransform;
	};
}

void OGUI::Context::PreparePrimitives(const OGUI::WindowHandle window)
{
	auto& wctx = GetWindowContext(window);
	auto root = wctx.GetWindowUI();
	wctx.currentDrawCtx = std::make_shared<PrimDrawContext>(wctx);
	nvgBeginFrame(wctx.currentDrawCtx->nvg, 1.0f);
	std::deque<DrawElement> queue;
	
	auto& ctx = *wctx.currentDrawCtx;
	std::vector<DrawList> drawLists;
	queue.push_back({0, root, {}, 0, 0});
	drawLists.emplace_back();
	while(!queue.empty())
	{
		auto next = queue.front();
		queue.pop_front();
		if(next.element->retained)
		{
			drawLists[next.drawPass].list.push_back(next);
			next.drawPass = drawLists.size();
			drawLists.emplace_back();
			next.clip[0] = next.clip[1] = {};
			next.clipNum = 0;
			drawLists[next.drawPass].inverseTransform = math::inverse(next.element->_worldTransform);
			if(next.element->_renderTarget)
			{
				drawLists[next.drawPass].renderTarget = next.element->_renderTarget;
				Bitmap bitmap;
				auto size = next.element->GetSize();
				bitmap.width = size.X;
				bitmap.height = size.Y;
				bitmap.format = PF_R8G8B8A8;
				renderImpl->UpdateRenderTargetView(next.element->_renderTarget, bitmap);
			}
			else 
			{
				Bitmap bitmap;
				auto size = next.element->GetSize();
				bitmap.width = size.X;
				bitmap.height = size.Y;
				bitmap.format = PF_R8G8B8A8;
				drawLists[next.drawPass].renderTarget = next.element->_renderTarget = renderImpl->RegisterRenderTargetView(bitmap);
			}
		}
		drawLists[next.drawPass].list.push_back(next);
		auto clippingChild = next.element->IsClippingChildren() && !next.element->retained;
		next.element->Traverse([&](VisualElement* child)
		{
			if(!child->Visible())
				return;
			DrawElement newDraw;
			newDraw.zorder = next.zorder + 1 + StylePosition::Get(child->_style).zOrderBias;
			newDraw.element = child;
			newDraw.clipNum = std::min((int)clippingChild + next.clipNum, (int)maxClipNum);
			newDraw.drawPass = next.drawPass;
			if(clippingChild)
			{
				newDraw.clip[1] = next.clip[0];
				newDraw.clip[0] = next.element->ApplyClipping();
			}
			else
			{
				newDraw.clip[1] = next.clip[1];
				newDraw.clip[0] = next.clip[0];
			}
			child->_opacity = StyleEffects::Get(child->_style).opacity * next.element->_opacity;
			queue.push_back(newDraw);
		});
	}
	for(auto& drawList : drawLists)
		std::stable_sort(drawList.list.begin(), drawList.list.end());
	ctx.prims.clear();
	for(auto iter = drawLists.rbegin(); iter != drawLists.rend(); ++iter)
	{
		ctx.current = &ctx.prims.emplace_back();
		if(iter->renderTarget)
		{
			ctx.current->renderTarget = iter->renderTarget;
			ctx.inverseTransform = &iter->inverseTransform;
		}
		for(auto& draw : iter->list)
		{
			ctx.clipStack.clear();
			for(int i=0; i<draw.clipNum; ++i)
				ctx.clipStack.push_back(draw.clip[i]);
			draw.element->DrawPrimitive(ctx);
		}
		ctx.inverseTransform = nullptr;
	}
	for(auto& prim : ctx.prims)
		prim.ValidateAndBatch();
}

void OGUI::Context::Render(const OGUI::WindowHandle window)
{
	auto& wctx = GetWindowContext(window);
	if(renderImpl.get()) 
		renderImpl->RenderPrimitives(*wctx.currentDrawCtx, wctx);
}

void OGUI::Context::MarkDirty(VisualElement* element, DirtyReason reason)
{

}

void OGUI::Context::UpdateHover(VisualElement* picked)
{
	if(picked == _elementUnderCursor)
		return;
	if (picked && _elementUnderCursor != picked)
	{
		PointerEnterEvent enterEvent;
		enterEvent.pointerType = u"mouse";
		enterEvent.gestureType = EGestureEvent::None;
		RouteEvent(picked, enterEvent);
	}
	if(_elementUnderCursor != nullptr && IsElementValid(_elementUnderCursor))
	{
		PointerLeaveEvent leaveEvent;
		leaveEvent.pointerType = u"mouse";
		leaveEvent.gestureType = EGestureEvent::None;
		RouteEvent(_elementUnderCursor, leaveEvent);
	}
	_elementUnderCursor = picked;
}

OGUI::VisualElement*  OGUI::Context::PickElement(const WindowHandle window, Vector2f point)
{
	if(_elementCapturingCursor)
		return _elementCapturingCursor;
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return nullptr;
	return PickRecursive(root, point);;
}

void OGUI::Context::CapturePointer(int id, VisualElement* element)
{
	auto root = element->GetRoot();
	if(!root->IsA(u"VisualWindow"))
		return;
	auto window = (VisualWindow*)root;
	inputImpl->CapturePointer(window->handle, true);
	_elementUnderCursor = _elementCapturingCursor = element;
	UpdateHover(_elementUnderCursor);
}

void OGUI::Context::ReleasePointer(int id)
{
	auto root = _elementCapturingCursor->GetRoot();
	_elementCapturingCursor = nullptr;
	if(!root->IsA(u"VisualWindow"))
		return;
	auto window = (VisualWindow*)root;
	inputImpl->CapturePointer(window->handle, false);
	if(_windowUnderCursor)
	{
		int x, y;
		inputImpl->GetCursorPos(x, y);
		int32 windowWidth = _windowUnderCursor->GetWidth(), windowHeight =  _windowUnderCursor->GetHeight();
		auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
		auto picked = PickElement(_windowUnderCursor, point);
		UpdateHover(picked);
	}
}

bool OGUI::Context::OnMouseDown(const OGUI::WindowHandle window, EMouseKey button, float x, float y)
{
	//olog::Info(u"OnMouseDown PosX:{0}, PosY:{1}"_o, x, y);
	int32 windowWidth = window->GetWidth(), windowHeight =  window->GetHeight();
	_windowUnderCursor = window;
	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	auto picked = PickElement(window, point);
	UpdateHover(picked);
	if(!picked)
		return false;
		
	YGNodeLayoutGetHadOverflow(picked->_ygnode); 
	PointerDownEvent event;
	pointerDownCount++;
	event.pointerType = u"mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;
	event.position = point;
	return RouteEvent(picked, event);
}

bool OGUI::Context::OnMouseUp(const OGUI::WindowHandle window, EMouseKey button, float x, float y)
{
	//olog::Info(u"OnMouseUp PosX:{0}, PosY:{1}"_o, x, y);
	int32 windowWidth = window->GetWidth(), windowHeight =  window->GetHeight();
	_windowUnderCursor = window;
	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	auto picked = PickElement(window, point);
	if (!picked)
		return false;
	PointerUpEvent event;
	event.pointerType = u"mouse";
	event.button = button;
	event.gestureType = EGestureEvent::None;
	event.position = point;
	return RouteEvent(picked, event);
}

bool OGUI::Context::OnMouseDoubleClick(const OGUI::WindowHandle window, EMouseKey button, float x, float y)
{
	int32 windowWidth = window->GetWidth(), windowHeight =  window->GetHeight();
	_windowUnderCursor = window;
	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	auto picked = PickElement(window, point);
	UpdateHover(picked);
	if(!picked)
		return false;
		
	YGNodeLayoutGetHadOverflow(picked->_ygnode); 
	PointerDoubleClickEvent event;
	event.pointerType = u"mouse";
	event.button = button;
	event.gestureType = EGestureEvent::None;
	event.position = point;
	return RouteEvent(picked, event);
}

bool OGUI::Context::OnMouseMove(const OGUI::WindowHandle window, float x, float y, float relativeMotionX, float relativeMotionY)
{
	_windowUnderCursor = window;
	int32 windowWidth = window->GetWidth(), windowHeight =  window->GetHeight();
	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	
	auto picked = PickElement(window, point);
	UpdateHover(picked);
	if (!picked)
		return false;
	PointerMoveEvent event;
	event.pointerType = u"mouse";
	event.gestureType = EGestureEvent::None;
	event.position = point;
	event.deltaPosition = Vector2f(relativeMotionX, relativeMotionY);
	return RouteEvent(picked, event);
}

bool OGUI::Context::OnMouseMoveHP(const OGUI::WindowHandle window, bool relative, float x, float y)
{
	_windowUnderCursor = window;
	//auto root = GetWindowContext(window).GetWindowUI();
	return false;
}

bool OGUI::Context::OnMouseWheel(const OGUI::WindowHandle window, float delta, float x, float y)
{
	if (!IsElementValid(_elementUnderCursor))
		return false;
	auto picked = _elementUnderCursor;
	int32 windowWidth = window->GetWidth(), windowHeight =  window->GetHeight();
	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	PointerScrollEvent event;
	event.pointerType = u"mouse";
	event.button = EMouseKey::MB;
	event.gestureType = EGestureEvent::None;
	event.wheelOrGestureDelta.y = delta;
	event.position = point;
	return RouteEvent(picked, event);
}

bool  OGUI::Context::OnMouseEnter(const WindowHandle window)
{
	_windowUnderCursor = window;
	return false;
}

bool  OGUI::Context::OnMouseLeave(const WindowHandle window)
{
	if(_windowUnderCursor == window)
		_windowUnderCursor = nullptr;
	return false;
}

bool OGUI::Context::OnKeyDown(const OGUI::WindowHandle window, EKeyCode keyCode)
{
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;

	if (_keyboardFocused)
	{
		bool result;
		KeyDownEvent e;
		e.key = keyCode;
		result = RouteEvent(_keyboardFocused, e);

		if(!result)
		{
			if(std::find(keyNavigation_Up.begin(), keyNavigation_Up.end(), keyCode) != keyNavigation_Up.end())
			{
				result = OnKeyNavigation(_keyboardFocused, ENavDirection::Up);
			}
			else if(std::find(keyNavigation_Down.begin(), keyNavigation_Down.end(), keyCode) != keyNavigation_Down.end())
			{
				result = OnKeyNavigation(_keyboardFocused, ENavDirection::Down);
			}
			else if(std::find(keyNavigation_Left.begin(), keyNavigation_Left.end(), keyCode) != keyNavigation_Left.end())
			{
				result = OnKeyNavigation(_keyboardFocused, ENavDirection::Left);
			}
			else if(std::find(keyNavigation_Right.begin(), keyNavigation_Right.end(), keyCode) != keyNavigation_Right.end())
			{
				result = OnKeyNavigation(_keyboardFocused, ENavDirection::Right);
			}

			if(!result)
			{
				KeyDownNavigationPostEvent ePost;
				ePost.key = keyCode;
				result = RouteEvent(_keyboardFocused, ePost);
			}
		}
		return result;
	}

	return false;
}

bool OGUI::Context::OnKeyUp(const OGUI::WindowHandle window, EKeyCode keyCode)
{
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;

	KeyUpEvent e;
	e.key = keyCode;

	if (_keyboardFocused)
		return RouteEvent(_keyboardFocused, e);

	return false;
}

bool OGUI::Context::OnAnalogValueChanged(const WindowHandle window, EKeyCode keyCode, float analogValue)
{
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;

	KeyAnalogInputEvent e;
	e.key = keyCode;
	e.analogValue = analogValue;

	if (_keyboardFocused)
		return RouteEvent(_keyboardFocused, e);

	return false;
}

void OGUI::Context::OnActivateWindow(const OGUI::WindowHandle window)
{
	auto root = GetWindowContext(window).GetWindowUI();
	ActivateWindow(root);
}

OGUI::Context::Context()
{
	ioThread = std::make_unique<IOThread>();
	logImpl = std::make_unique<StdOutputLog>();
	RegisterOGUIXmlParser();
}

OGUI::Context::~Context()
{
}

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}

OGUI::WindowContext& OGUI::Context::GetOrRegisterWindowContext(const OGUI::WindowHandle window)
{
	for(auto&& ctx : windowContexts)
	{
		if(ctx->window == window)
			return *ctx.get();
	}
	WindowContext& newOne = *windowContexts.emplace_back(std::make_unique<WindowContext>(window)).get();
	return newOne;
}

OGUI::WindowContext& OGUI::Context::GetWindowContext(const OGUI::WindowHandle window)
{
	for(auto& ctx : windowContexts)
	{
		if(ctx->window == window)
			return *ctx.get();
	}
	// warn("window context not found")
	static OGUI::WindowContext NULL_WINDOW_CONTEXT = OGUI::WindowContext(nullptr);
	return NULL_WINDOW_CONTEXT;
}

bool OGUI::Context::OnKeyNavigation(OGUI::VisualElement* element, ENavDirection direction)
{
	auto nextNavTarget = element->FindNextNavTarget(direction);
	if(nextNavTarget)
	{
		SetFocus(nextNavTarget, EFocusChangeCause::FocusNavigation);
		return true;
	}
	return false;
}

bool OGUI::Context::ActivateWindow(OGUI::VisualWindow* newWindow)
{
	if(newWindow != _currentActivateWindow)
	{
		std::vector<VisualElement*> FocusedPath;
		VisualWindow::GetRelativeFocusedPath(newWindow, FocusedPath);
		VisualElement* newKeyboardFocused = FocusedPath.back();

		if(_keyboardFocused)
		{
			PreLostKeyboardFocusEvent preLostKeyboardFocusEvent;
			preLostKeyboardFocusEvent.cause = EFocusChangeCause::ActivateWindow;
			preLostKeyboardFocusEvent.causeDescribe = u"";
			preLostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			preLostKeyboardFocusEvent.newFocused = newKeyboardFocused;
			if(RouteEvent(_keyboardFocused, preLostKeyboardFocusEvent)) return false;
		}

		PreGotKeyboardFocusEvent preGotKeyboardFocusEvent;
		preGotKeyboardFocusEvent.cause = EFocusChangeCause::ActivateWindow;
		preGotKeyboardFocusEvent.causeDescribe = u"";
		preGotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		preGotKeyboardFocusEvent.newFocused = newKeyboardFocused;
		if(RouteEvent(newKeyboardFocused, preGotKeyboardFocusEvent)) return false;
		auto oldKeyboardFocused = _keyboardFocused;
		_keyboardFocused = newKeyboardFocused;
		_currentActivateWindow = newWindow;

		if(oldKeyboardFocused)
		{
			LostKeyboardFocusEvent lostKeyboardFocusEvent;
			lostKeyboardFocusEvent.cause = EFocusChangeCause::ActivateWindow;
			lostKeyboardFocusEvent.causeDescribe = u"";
			lostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			lostKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
			RouteEvent(oldKeyboardFocused, lostKeyboardFocusEvent);
		}

		GotKeyboardFocusEvent gotKeyboardFocusEvent;
		gotKeyboardFocusEvent.cause = EFocusChangeCause::ActivateWindow;
		gotKeyboardFocusEvent.causeDescribe = u"";
		gotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		gotKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
		RouteEvent(_keyboardFocused, gotKeyboardFocusEvent);
		return true;
		
	}
	return false;
}

bool OGUI::Context::SetFocus(OGUI::VisualElement* element, EFocusChangeCause cause, ostr::string describe)
{
	if(!element->focusable) return false;

	auto newFocus_Root = element->GetRoot();
	VisualElement* oldKeyboardFocus = _keyboardFocused;
	VisualElement* newKeyboardFocus = newFocus_Root == _currentActivateWindow ? element : nullptr;

	std::vector<VisualElement*> oldFocusedPath;
	VisualWindow::GetRelativeFocusedPath(newFocus_Root, oldFocusedPath);

	std::vector<VisualElement*> newFocusedPath;
	VisualElement* tempNewFocused = element;
	while (tempNewFocused) 
	{
		newFocusedPath.insert(newFocusedPath.begin(), tempNewFocused);
		tempNewFocused = tempNewFocused->GetPrevFocusScope();
	}

	// 焦点丢失前事件
	for (int i = 0; i < oldFocusedPath.size(); ++i) 
	{
		VisualElement* oldFocused = oldFocusedPath[i];
		VisualElement* newFocused = i < newFocusedPath.size() ? newFocusedPath[i] : nullptr;

		if(!IsElementValid(oldFocused))
			continue;

		if(oldFocused && oldFocused != newFocused)
		{
			PreLostFocusEvent preLostFocusEvent;
			preLostFocusEvent.cause = cause;
			preLostFocusEvent.causeDescribe = describe;
			preLostFocusEvent.currentFocusedPath = &oldFocusedPath;
			preLostFocusEvent.newFocusedPath = &newFocusedPath;
			if(RouteEvent(oldFocused, preLostFocusEvent)) return false;
			if(!oldFocused->isFocusScope) break;
			if(oldFocused->isFocusScope && oldFocused->isKeeyScopeFocused) break;
		}
	}
	if(oldKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		PreLostKeyboardFocusEvent preLostKeyboardFocusEvent;
		preLostKeyboardFocusEvent.cause = cause;
		preLostKeyboardFocusEvent.causeDescribe = describe;
		preLostKeyboardFocusEvent.currentFocused = oldKeyboardFocus;
		preLostKeyboardFocusEvent.newFocused = newKeyboardFocus;
		if(RouteEvent(oldKeyboardFocus, preLostKeyboardFocusEvent)) return false;
	}
	for (int i = 0; i < newFocusedPath.size(); ++i) 
	{
		VisualElement* oldFocused = i < oldFocusedPath.size() ? oldFocusedPath[i] : nullptr;
		VisualElement* prevNewFocused = i-1 >= 0 ? newFocusedPath[i-1] : nullptr;
		VisualElement* newFocused = newFocusedPath[i];

		auto prevCurrentFocused = prevNewFocused ? prevNewFocused->GetFocusScopeFocused() : nullptr;
		if(prevNewFocused && oldFocused != newFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevCurrentFocused &&
			prevCurrentFocused != newFocused)
		{
			PreLostFocusEvent preLostFocusEvent;
			preLostFocusEvent.cause = cause;
			preLostFocusEvent.causeDescribe = describe;
			preLostFocusEvent.currentFocusedPath = &oldFocusedPath;
			preLostFocusEvent.newFocusedPath = &newFocusedPath;
			if(RouteEvent(prevCurrentFocused, preLostFocusEvent)) return false;
		}
	}

	//焦点获得前事件
	for (int i = 0; i < newFocusedPath.size(); ++i) 
	{
		VisualElement* oldFocused = i < oldFocusedPath.size() ? oldFocusedPath[i] : nullptr;
		VisualElement* prevNewFocused = i-1 >= 0 ? newFocusedPath[i-1] : nullptr;
		VisualElement* newFocused = newFocusedPath[i];

		auto prevCurrentFocused = prevNewFocused ? prevNewFocused->GetFocusScopeFocused() : nullptr;
		if(oldFocused != newFocused)
		{
			if(prevNewFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevCurrentFocused &&
			prevCurrentFocused == newFocused)
				continue;

			PreGotFocusEvent preGotFocusEvent;
			preGotFocusEvent.cause = cause;
			preGotFocusEvent.causeDescribe = describe;
			preGotFocusEvent.currentFocusedPath = &oldFocusedPath;
			preGotFocusEvent.newFocusedPath = &newFocusedPath;
			if(RouteEvent(newFocused, preGotFocusEvent)) return false;
		}
	}
	if(newKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		PreGotKeyboardFocusEvent preGotKeyboardFocusEvent;
		preGotKeyboardFocusEvent.cause = cause;
		preGotKeyboardFocusEvent.causeDescribe = describe;
		preGotKeyboardFocusEvent.currentFocused = oldKeyboardFocus;
		preGotKeyboardFocusEvent.newFocused = newKeyboardFocus;
		if(RouteEvent(newKeyboardFocus, preGotKeyboardFocusEvent)) return false;
	}

	//离开焦点事件
	for (int i = 0; i < oldFocusedPath.size(); ++i) 
	{
		VisualElement* prevOldFocused = i-1 >= 0 ? oldFocusedPath[i-1] : nullptr;
		VisualElement* oldFocused = oldFocusedPath[i];
		VisualElement* newFocused = i < newFocusedPath.size() ? newFocusedPath[i] : nullptr;

		if(oldFocused && oldFocused != newFocused)
		{
			if(prevOldFocused)
				prevOldFocused->currentFocused = nullptr; //实际修改状态

			LostFocusEvent lostFocusEvent;
			lostFocusEvent.cause = cause;
			lostFocusEvent.causeDescribe = describe;
			lostFocusEvent.currentFocusedPath = &newFocusedPath;
			lostFocusEvent.oldFocusedPath = &oldFocusedPath;
			RouteEvent(oldFocused, lostFocusEvent);

			if(!oldFocused->isFocusScope) break;
			if(oldFocused->isFocusScope && oldFocused->isKeeyScopeFocused) break;
		}
	}
	if(oldKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		LostKeyboardFocusEvent lostKeyboardFocusEvent;
		lostKeyboardFocusEvent.cause = cause;
		lostKeyboardFocusEvent.causeDescribe = describe;
		lostKeyboardFocusEvent.currentFocused = newKeyboardFocus;
		lostKeyboardFocusEvent.oldFocused = oldKeyboardFocus;
		RouteEvent(oldKeyboardFocus, lostKeyboardFocusEvent);
	}
	for (int i = 0; i < newFocusedPath.size(); ++i) 
	{
		VisualElement* oldFocused = i < oldFocusedPath.size() ? oldFocusedPath[i] : nullptr;
		VisualElement* prevNewFocused = i-1 >= 0 ? newFocusedPath[i-1] : nullptr;
		VisualElement* newFocused = newFocusedPath[i];

		auto prevCurrentFocused = prevNewFocused ? prevNewFocused->GetFocusScopeFocused() : nullptr;
		if(prevNewFocused && oldFocused != newFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevCurrentFocused &&
			prevCurrentFocused != newFocused)
		{
			LostFocusEvent lostFocusEvent;
			lostFocusEvent.cause = cause;
			lostFocusEvent.causeDescribe = describe;
			lostFocusEvent.currentFocusedPath = &newFocusedPath;
			lostFocusEvent.oldFocusedPath = &oldFocusedPath;
			if(RouteEvent(prevCurrentFocused, lostFocusEvent)) return false;
		}
	}

	//获得焦点事件
	for (int i = 0; i < newFocusedPath.size(); ++i) 
	{
		VisualElement* oldFocused = i < oldFocusedPath.size() ? oldFocusedPath[i] : nullptr;
		VisualElement* prevNewFocused = i-1 >= 0 ? newFocusedPath[i-1] : nullptr;
		VisualElement* newFocused = newFocusedPath[i];

		if(oldFocused != newFocused)
		{
			if(prevNewFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevNewFocused->currentFocused &&
			prevNewFocused->currentFocused == newFocused)
				continue;

			if(prevNewFocused)
				prevNewFocused->currentFocused = newFocused; //实际修改状态

			GotFocusEvent gotFocusEvent;
			gotFocusEvent.cause = cause;
			gotFocusEvent.causeDescribe = describe;
			gotFocusEvent.currentFocusedPath = &newFocusedPath;
			gotFocusEvent.oldFocusedPath = &oldFocusedPath;
			RouteEvent(newFocused, gotFocusEvent);
		}
	}
	if(newKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		_keyboardFocused = newKeyboardFocus; //实际修改状态

		GotKeyboardFocusEvent gotKeyboardFocusEvent;
		gotKeyboardFocusEvent.cause = cause;
		gotKeyboardFocusEvent.causeDescribe = describe;
		gotKeyboardFocusEvent.currentFocused = newKeyboardFocus;
		gotKeyboardFocusEvent.oldFocused = oldKeyboardFocus;
		RouteEvent(newKeyboardFocus, gotKeyboardFocusEvent);
	}

	return true;
}

void OGUI::Context::SetXmlFilter_Global(const char* key, const char* filterTag)
{
	auto result = _globalXmlFiltersMap.try_emplace(key, filterTag);
	bool isSet = result.first->second != filterTag;
	if(isSet)
		result.first->second = filterTag;

	if(result.second || isSet)
	{
		ForceRefreshXmlFilter();
	}
}

void OGUI::Context::CleanXmlFilter_Global(const char* key)
{
	if(_globalXmlFiltersMap.erase(key) > 0)
	{
		ForceRefreshXmlFilter();
	}
}

void OGUI::Context::ResetXmlFilter_Global()
{
	_globalXmlFiltersMap.clear();
	ForceRefreshXmlFilter();
}

void OGUI::Context::UpdataXmlFilterCache_Global()
{
	_globalXmlFiltersCache.clear();
	for(auto pair : _globalXmlFiltersMap)
		_globalXmlFiltersCache.insert(pair.second);
}

bool OGUI::Context::HasFilterTag_Global(const char* filterTag) const
{
	return HasFilterTag_Global(Name(filterTag));
}

bool OGUI::Context::HasFilterTag_Global(Name filterTag) const
{
	return _globalXmlFiltersCache.count(filterTag);
}

void OGUI::Context::ForceRefreshXmlFilter()
{
	UpdataXmlFilterCache_Global();
	std::map<Name, int> localXmlFilters;
	for(auto& winContext : windowContexts)
	{
		localXmlFilters.clear();
		RecursionUpdataFilter(winContext->ui, localXmlFilters);
	}
}

bool OGUI::Context::UpdataFilter(VisualElement* element, std::map<Name, int>& localXmlFilters)
{
	for(const auto& filterTag : element->_xmlFilters)
	{
		if(!localXmlFilters.count(filterTag) && !HasFilterTag_Global(filterTag))
		{
			if(element->_visible)
			{
				element->SetVisibility(false);
			}
			return false;
		}
	}
	if(!element->_visible)
	{
		element->SetVisibility(true);
	}
	MarkLayoutDirty(element);
	return true;
}

void OGUI::Context::RecursionUpdataFilter(VisualElement* element, std::map<Name, int>& localXmlFilters)
{
	for(auto localFilterTag : element->_localXmlFiltersCache)
	{
		auto result = localXmlFilters.try_emplace(localFilterTag);
		result.first->second += 1;
	}

	if(!UpdataFilter(element, localXmlFilters))
			return;
	element->Traverse([this, &localXmlFilters](VisualElement* next)
	{
		if(!UpdataFilter(next, localXmlFilters))
			return;
		RecursionUpdataFilter(next, localXmlFilters);
	});

	for(auto localFilterTag : element->_localXmlFiltersCache)
	{
		auto result = localXmlFilters.try_emplace(localFilterTag);
		if(--(result.first->second) == 0)
			localXmlFilters.erase(result.first);
	}
}

bool OGUI::Context::IsElementValid(OGUI::VisualElement* e) const
{
	return _allElementHandle.find(e) != _allElementHandle.end();
}