#include <memory>
#define DLL_IMPLEMENTATION
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

#include "OpenGUI/Core/ostring/ostr.h"
#include "OpenGUI/Core/olog.h"
#include "OpenGUI/Core/StdLog.h"
#include "Text/godot/text_server_adv.h"

OGUI::WindowContext::WindowContext()
{
	textureManager = std::make_unique<RenderTextureManager>(*this);
	ui = new VisualWindow();
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

namespace OGUI
{
	void RenderRec(VisualElement* element, PrimitiveDraw::DrawContext& ctx)
	{
		element->DrawPrimitive(ctx);
		element->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
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
		for (auto& child : element->_children)
			if(auto picked = PickRecursive(child, point))
				return picked;
		auto invTransform = math::inverse(element->_worldTransform);
		Vector4f dummy = {point.X, point.Y, 0.f, 1.f};
		const Vector4f result = math::multiply(dummy, invTransform);
		Vector2f localPoint = {result.X, result.Y};

		//std::cout << "OnMouseDown: " << localPoint.X << "," << localPoint.Y << std::endl;
		//std::cout << "Name: " << element->_name << std::endl;
		//std::cout << "Rect: " << element->GetRect().min.X << element->GetRect().min.Y << std::endl;
		if (element->Intersect(localPoint))
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
	void UpdateLayout(VisualElement* element)
	{
		auto& ctx = Context::Get();
		if (ctx._layoutDirty)
		{
			CacheLayoutRec(element);
			element->CalculateLayout();
			CheckLayoutRec(element);
			ctx._layoutDirty = false;
		}
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
	wctx.textureManager->Update();
	// Update Window
	inputImpl->GetWindowProperties(window, wctx.X, wctx.Y);	
	_deltaTime = dt;
	animSystem.Update(root);
	styleSystem.Update(root);
	UpdateLayout(root);
	TransformRec(root);
}

void OGUI::Context::Render(const OGUI::WindowHandle window)
{
	auto& wctx = GetWindowContext(window);
	auto root = wctx.GetWindowUI();
	wctx.currentDrawCtx = std::make_shared<PrimitiveDraw::DrawContext>(PrimitiveDraw::DrawContext{wctx});
	wctx.currentDrawCtx->resolution = Vector2f(wctx.X, wctx.Y);
	root->Traverse([&](VisualElement* next) { RenderRec(next, *wctx.currentDrawCtx); });
	if(wctx.renderImpl) wctx.renderImpl->RenderPrimitives(wctx.currentDrawCtx->prims, wctx);
}

void OGUI::Context::MarkDirty(VisualElement* element, DirtyReason reason)
{

}

bool OGUI::Context::OnMouseDown(const OGUI::WindowHandle window, float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y)
{
	olog::Info(u"OnMouseDown PosX:{0}, PosY:{1}"_o, x, y);
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;
	PointerDownEvent event;
	//todo: what is mouse up is missing?
	pointerDownCount++;
	event.pointerType = "mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;

	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window

	auto picked = PickRecursive(root, point);
	if (picked)
	{
		RouteEvent(picked, event);
	}
	return false;
}

bool OGUI::Context::OnMouseUp(const OGUI::WindowHandle window, float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y)
{
	auto root = GetWindowContext(window).GetWindowUI();
	if (!root)
		return false;
	PointerUpEvent event;

	pointerDownCount--;
	event.pointerType = "mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;

	auto point = Vector2f(x, windowHeight - y) - Vector2f(windowWidth, windowHeight) / 2; // center of the window
	//printf("X: %.2f, Y: %.2f\n", point.X, point.Y);
	//olog::info("Mouse Up");

	//auto picked = PickRecursive(root, point);
	//if (picked)
	//{
	//	if (picked != currentFocus)
	//	{
	//		picked->SetPseudoClass(PseudoStates::Focus, false);
	//		if (auto currF = currentFocus)
	//		{
	//			currF->SetPseudoClass(PseudoStates::Focus, false);
	//		}
	//	}
	//	currentFocus = picked;
	//	RouteEvent(picked, event);
	//}
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(const OGUI::WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	//auto root = GetWindowContext(window).GetWindowUI();
	//std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseMove(const OGUI::WindowHandle window, bool relative, int32 x, int32 y)
{
	//auto root = GetWindowContext(window).GetWindowUI();
	//olog::Info(u"Mouse PosX:{0}, PosY:{1}"_o, x, y);
	return false;
}

bool OGUI::Context::OnMouseMoveHP(const OGUI::WindowHandle window, bool relative, float x, float y)
{
	//auto root = GetWindowContext(window).GetWindowUI();
	return false;
}

bool OGUI::Context::OnMouseWheel(const OGUI::WindowHandle window, float delta)
{
	auto root = GetWindowContext(window).GetWindowUI();
	olog::Info(u"Mouse WheelY:{}"_o, delta);
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
				OnKeyNavigation(_keyboardFocused, ENavDirection::Up);
			}
			else if(std::find(keyNavigation_Down.begin(), keyNavigation_Down.end(), keyCode) != keyNavigation_Down.end())
			{
				OnKeyNavigation(_keyboardFocused, ENavDirection::Down);
			}
			else if(std::find(keyNavigation_Left.begin(), keyNavigation_Left.end(), keyCode) != keyNavigation_Left.end())
			{
				OnKeyNavigation(_keyboardFocused, ENavDirection::Left);
			}
			else if(std::find(keyNavigation_Right.begin(), keyNavigation_Right.end(), keyCode) != keyNavigation_Right.end())
			{
				OnKeyNavigation(_keyboardFocused, ENavDirection::Right);
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
}

OGUI::Context::~Context()
{
}

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}

static OGUI::WindowContext NULL_WINDOW_CONTEXT = OGUI::WindowContext();
OGUI::WindowContext& OGUI::Context::GetOrRegisterWindowContext(const OGUI::WindowHandle window)
{
	for(auto&& ctx : windowContexts)
	{
		if(ctx->window == window)
			return *ctx.get();
	}
	WindowContext& newOne = *windowContexts.emplace_back(std::make_unique<WindowContext>()).get();
	newOne.window = window;
	newOne.X = window->GetWidth();
	newOne.Y = window->GetHeight();
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
	return NULL_WINDOW_CONTEXT;
}

void OGUI::Context::OnKeyNavigation(OGUI::VisualElement* element, ENavDirection direction)
{
	auto nextNavTarget = element->FindNextNavTarget(direction);
	if(nextNavTarget)
	{
		SetFocus(nextNavTarget, FocusChangeCause::FocusNavigation);
	}
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