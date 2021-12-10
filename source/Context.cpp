
#include "Yoga.h"
#include <algorithm>
#include <cstdio>
#include <vector>
#include <memory>
#include "OpenGUI/Configure.h"
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
	void RenderRec(VisualElement* element, PrimitiveDraw::DrawContext& ctx)
	{
		if(!element->Visible())
			return;
		auto clippingChild = element->IsClippingChildren();
		if(clippingChild)
			ctx.prims.clipStack.push_back(element->ApplyClipping());
		element->DrawPrimitive(ctx);
		element->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
		if(clippingChild)
			ctx.prims.clipStack.pop_back();
	}
	void TransformRec(VisualElement* element, bool dirty = false)
	{
		dirty |= element->_transformDirty;
		if(dirty)
			element->UpdateWorldTransform();
		element->Traverse([&](VisualElement* next) { TransformRec(next, dirty); });
	}

	VisualElement* PickRecursive(VisualElement* element, Vector2f point)
	{
		if(!element->Visible())
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
			element->_transformDirty = true;
		else
			element->Traverse([&](VisualElement* next) { CheckLayoutRec(next); });
	}
	void UpdateLayout(VisualElement* element, float width, float height)
	{
		auto& ctx = Context::Get();
		if(!ctx._layoutDirty)
			return;
		CacheLayoutRec(element);
		while (ctx._layoutDirty)
		{
			element->CalculateLayout(width, height);
			ctx._layoutDirty = false;
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
			windowContexts.erase(windowContexts.begin() + i);
			return;
		}
	}
}
 
void OGUI::Context::Update(const OGUI::WindowHandle window, float dt)
{
	auto& wctx = GetOrRegisterWindowContext(window);
	auto root = wctx.GetWindowUI();
	// Texture Streaming
	textureManager->Update();
	_deltaTime = dt;
	styleSystem.Update(root);
	UpdateLayout(root, wctx.GetWidth(), wctx.GetHeight());
	UpdateScrollSize(root);
	TransformRec(root);
	UpdateVisibility(root);
}

void OGUI::Context::PreparePrimitives(const OGUI::WindowHandle window)
{
	auto& wctx = GetWindowContext(window);
	auto root = wctx.GetWindowUI();
	wctx.currentDrawCtx = std::make_shared<PrimitiveDraw::DrawContext>(PrimitiveDraw::DrawContext{wctx});
	root->Traverse([&](VisualElement* next) { RenderRec(next, *wctx.currentDrawCtx); });
	wctx.currentDrawCtx->prims.ValidateAndBatch();
}

void OGUI::Context::Render(const OGUI::WindowHandle window)
{
	auto& wctx = GetWindowContext(window);
	if(renderImpl.get()) 
		renderImpl->RenderPrimitives(wctx.currentDrawCtx->prims, wctx);
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
		enterEvent.pointerType = "mouse";
		enterEvent.gestureType = EGestureEvent::None;
		RouteEvent(picked, enterEvent);
	}
	if(_elementUnderCursor != nullptr && IsElementValid(_elementUnderCursor))
	{
		PointerLeaveEvent leaveEvent;
		leaveEvent.pointerType = "mouse";
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
	if(!root->IsA("VisualWindow"))
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
	if(!root->IsA("VisualWindow"))
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
	event.pointerType = "mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;
	event.position = point;
	RouteEvent(picked, event);
	return true;
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
	event.pointerType = "mouse";
	event.button = button;
	event.gestureType = EGestureEvent::None;
	event.position = point;
	RouteEvent(picked, event);
	return true;
}

bool OGUI::Context::OnMouseDoubleClick(const OGUI::WindowHandle window, EMouseKey button, float x, float y)
{
	_windowUnderCursor = window;
	//auto root = GetWindowContext(window).GetWindowUI();
	//std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
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
	event.pointerType = "mouse";
	event.gestureType = EGestureEvent::None;
	event.position = point;
	RouteEvent(picked, event);
	return false;
}

bool OGUI::Context::OnMouseMoveHP(const OGUI::WindowHandle window, bool relative, float x, float y)
{
	_windowUnderCursor = window;
	//auto root = GetWindowContext(window).GetWindowUI();
	return false;
}

bool OGUI::Context::OnMouseWheel(const OGUI::WindowHandle window, float delta)
{
	auto picked = _elementUnderCursor;
	if (!picked)
		return false;
	PointerScrollEvent event;
	event.pointerType = "mouse";
	event.button = EMouseKey::MB;
	event.gestureType = EGestureEvent::None;
	event.wheelOrGestureDelta.y = delta;
	RouteEvent(picked, event);
	return false;
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

static bool gPrevPressed = false;

bool OGUI::Context::OnKeyDown(const OGUI::WindowHandle window, EKeyCode keyCode)
{
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;

	if (_keyboardFocused)
	{
		bool result;
		if (gPrevPressed == false)
		{
			KeyDownEvent e;
			e.key = keyCode;
			result = RouteEvent(_keyboardFocused, e);
			gPrevPressed = true;
		}
		else
		{
			KeyHoldEvent e;
			e.key = keyCode;
			result = RouteEvent(_keyboardFocused, e);
		}

		if(!result)
		{
			if(std::find(keyNavigation_Up.begin(), keyNavigation_Up.end(), keyCode) != keyNavigation_Up.end())
			{
				return OnKeyNavigation(_keyboardFocused, ENavDirection::Up);
			}
			else if(std::find(keyNavigation_Down.begin(), keyNavigation_Down.end(), keyCode) != keyNavigation_Down.end())
			{
				return OnKeyNavigation(_keyboardFocused, ENavDirection::Down);
			}
			else if(std::find(keyNavigation_Left.begin(), keyNavigation_Left.end(), keyCode) != keyNavigation_Left.end())
			{
				return OnKeyNavigation(_keyboardFocused, ENavDirection::Left);
			}
			else if(std::find(keyNavigation_Right.begin(), keyNavigation_Right.end(), keyCode) != keyNavigation_Right.end())
			{
				return OnKeyNavigation(_keyboardFocused, ENavDirection::Right);
			}
		}
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
		RouteEvent(_keyboardFocused, e);
	gPrevPressed = false;

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
	RegisterBaseAttrConverter();
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
		SetFocus(nextNavTarget, FocusChangeCause::FocusNavigation);
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
			preLostKeyboardFocusEvent.cause = FocusChangeCause::ActivateWindow;
			preLostKeyboardFocusEvent.causeDescribe = "";
			preLostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			preLostKeyboardFocusEvent.newFocused = newKeyboardFocused;
			if(RouteEvent(_keyboardFocused, preLostKeyboardFocusEvent)) return false;
		}

		PreGotKeyboardFocusEvent preGotKeyboardFocusEvent;
		preGotKeyboardFocusEvent.cause = FocusChangeCause::ActivateWindow;
		preGotKeyboardFocusEvent.causeDescribe = "";
		preGotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		preGotKeyboardFocusEvent.newFocused = newKeyboardFocused;
		if(RouteEvent(newKeyboardFocused, preGotKeyboardFocusEvent)) return false;
		auto oldKeyboardFocused = _keyboardFocused;
		_keyboardFocused = newKeyboardFocused;
		_currentActivateWindow = newWindow;

		if(oldKeyboardFocused)
		{
			LostKeyboardFocusEvent lostKeyboardFocusEvent;
			lostKeyboardFocusEvent.cause = FocusChangeCause::ActivateWindow;
			lostKeyboardFocusEvent.causeDescribe = "";
			lostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			lostKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
			RouteEvent(oldKeyboardFocused, lostKeyboardFocusEvent);
		}

		GotKeyboardFocusEvent gotKeyboardFocusEvent;
		gotKeyboardFocusEvent.cause = FocusChangeCause::ActivateWindow;
		gotKeyboardFocusEvent.causeDescribe = "";
		gotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		gotKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
		RouteEvent(_keyboardFocused, gotKeyboardFocusEvent);
		return true;
		
	}
	return false;
}

bool OGUI::Context::SetFocus(OGUI::VisualElement* element, FocusChangeCause cause, std::string describe)
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

		if(prevNewFocused && oldFocused != newFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevNewFocused->currentFocused &&
			prevNewFocused->currentFocused != newFocused)
		{
			PreLostFocusEvent preLostFocusEvent;
			preLostFocusEvent.cause = cause;
			preLostFocusEvent.causeDescribe = describe;
			preLostFocusEvent.currentFocusedPath = &oldFocusedPath;
			preLostFocusEvent.newFocusedPath = &newFocusedPath;
			if(RouteEvent(prevNewFocused->currentFocused, preLostFocusEvent)) return false;
		}
	}

	//焦点获得前事件
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
			if(prevOldFocused) prevOldFocused->currentFocused = nullptr; //实际修改状态

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

		if(prevNewFocused && oldFocused != newFocused && 
			prevNewFocused->isFocusScope && 
			prevNewFocused->isKeeyScopeFocused && 
			prevNewFocused->currentFocused &&
			prevNewFocused->currentFocused != newFocused)
		{
			LostFocusEvent lostFocusEvent;
			lostFocusEvent.cause = cause;
			lostFocusEvent.causeDescribe = describe;
			lostFocusEvent.currentFocusedPath = &newFocusedPath;
			lostFocusEvent.oldFocusedPath = &oldFocusedPath;
			if(RouteEvent(prevNewFocused->currentFocused, lostFocusEvent)) return false;
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

			if(prevNewFocused) prevNewFocused->currentFocused = newFocused; //实际修改状态

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

void OGUI::Context::SetXmlFilter(const char* key, const char* filterTag)
{
	auto result = _xmlFiltersMap.try_emplace(key, filterTag);
	if(result.second)
	{
		UpdataXmlFilterCache();
		for(auto& winContext : windowContexts)
			RecursionUpdataFilter(winContext->ui);
	}
	else if(result.first->second != filterTag)
	{
		result.first->second = filterTag;
		UpdataXmlFilterCache();
		for(auto& winContext : windowContexts)
			RecursionUpdataFilter(winContext->ui);
	}
}

void OGUI::Context::AddXmlFilter(const char* filterTag)
{
	auto result = _xmlFilters.emplace(filterTag);
	if(result.second)
	{
		UpdataXmlFilterCache();
		for(auto& winContext : windowContexts)
			RecursionUpdataFilter(winContext->ui);
	}
}

void OGUI::Context::RemoveXmlFilter(const char* filterTag)
{
	if(_xmlFilters.erase(filterTag))
	{
		UpdataXmlFilterCache();
		for(auto& winContext : windowContexts)
			RecursionUpdataFilter(winContext->ui);
	}
}

void OGUI::Context::UpdataXmlFilterCache()
{
	_xmlFiltersCache.clear();
	_xmlFiltersCache.insert(_xmlFilters.begin(), _xmlFilters.end());
	for(auto pair : _xmlFiltersMap)
		_xmlFiltersCache.insert(pair.second);
}

bool OGUI::Context::HasFilterTag(const char* filterTag) const
{
	return HasFilterTag(Name(filterTag));
}

bool OGUI::Context::HasFilterTag(Name filterTag) const
{
	return _xmlFiltersCache.count(filterTag);
}

bool OGUI::Context::UpdataFilter(VisualElement* element)
{
	for(const auto& filterTag : element->_xmlFilters)
	{
		if(!HasFilterTag(filterTag))
		{
			if(element->Visible())
			{
				_layoutDirty = true;
				styleSystem.InvalidateCache();
				element->SetVisibility(false);
			}
			return false;
		}
	}
	if(!element->Visible())
	{
		_layoutDirty = true;
		styleSystem.InvalidateCache();
		element->SetVisibility(true);
	}
	return true;
}

void OGUI::Context::RecursionUpdataFilter(VisualElement* element)
{
	if(!UpdataFilter(element))
			return;
	element->Traverse([this](VisualElement* next)
	{
		if(!UpdataFilter(next))
			return;
		RecursionUpdataFilter(next);
	});
}

bool OGUI::Context::IsElementValid(OGUI::VisualElement* e) const
{
	return _allElementHandle.find(e) != _allElementHandle.end();
}