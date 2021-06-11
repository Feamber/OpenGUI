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

void OGUI::Context::Initialize(
	InputInterface* I, SystemInterface* S,
	RenderInterface* R, FileInterface* F,
	BitmapParserInterface* B)
{
	OASSERT(initialized);
	inputImpl.reset(I);
	systemImpl.reset(S);
	renderImpl.reset(R);
	if(F)
	{
		fileImpl.reset(F);
	} else {
		fileImpl = std::make_unique<FileInterface>();
	}
	
	bmParserImpl.reset(B);
	ioThread = std::make_unique<OGUI::IOThread>();

	initialized = true;
}

namespace OGUI
{
	void RenderRec(VisualElement* element, PrimitiveDraw::DrawContext& ctx)
	{
		element->DrawPrimitive(ctx);
		element->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
	}
	void TransformRec(VisualElement* element)
	{
		bool dirty = element->_transformDirty;
		if(dirty)
			element->UpdateWorldTransform();
		element->Traverse([&](VisualElement* next) { if(dirty) next->_transformDirty = true; TransformRec(next); });
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
			element->Traverse([&](VisualElement* next) { CacheLayoutRec(next); });
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

void OGUI::Context::Update(const WindowHandle window, float dt)
{
	auto root = desktops;
	textureManager->Update();
	// Update Window
	auto& wctx = GetOrRegisterWindowContext(window);
	inputImpl->GetWindowProperties(window, wctx.X, wctx.Y);	
	_deltaTime = dt;
	animSystem.Update(root);
	styleSystem.Update(root);
	UpdateLayout(root);
	TransformRec(root);
}

void OGUI::Context::Render(const WindowHandle window)
{
	auto root = desktops;
	PrimitiveDraw::DrawContext ctx;
	const auto& wctx = GetWindowContext(window);
	ctx.resolution = Vector2f(wctx.X, wctx.Y);
	root->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
	renderImpl->RenderPrimitives(ctx.prims);
}

void OGUI::Context::MarkDirty(VisualElement* element, DirtyReason reason)
{

}

bool OGUI::Context::OnMouseDown(float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y)
{
	auto root = desktops;
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

bool OGUI::Context::OnMouseUp(float windowWidth, float windowHeight, EMouseKey button, int32 x, int32 y)
{
	auto root = desktops;
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

bool OGUI::Context::OnMouseDoubleClick(EMouseKey button, int32 x, int32 y)
{
	//std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseMove(bool relative, int32 x, int32 y)
{
	olog::Info(u"Mouse PosX:%d, PosY:%d"_o, x, y);
	return false;
}

bool OGUI::Context::OnMouseMoveHP(bool relative, float x, float y)
{
	return false;
}

bool OGUI::Context::OnMouseWheel(float delta)
{
	olog::Info(u"Mouse WheelY:{}"_o, delta);
	return false;
}

static bool gPrevPressed = false;

bool OGUI::Context::OnKeyDown(EKeyCode keyCode)
{
	auto root = desktops;
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

bool OGUI::Context::OnKeyUp(EKeyCode keyCode)
{
	auto root = desktops;
	if (!root)
		return false;

	KeyUpEvent e;
	e.key = keyCode;

	if (_keyboardFocused)
		RouteEvent(_keyboardFocused, e);
	gPrevPressed = false;

	return false;
}

OGUI::Context::Context()
{
	ioThread = std::make_unique<IOThread>();
	textureManager = std::make_unique<RenderTextureManager>();
	logImpl = std::make_unique<StdOutputLog>();
}

OGUI::Context::~Context()
{
	if (desktops)
		VisualElement::DestoryTree(desktops);
	if (dialogs)
		VisualElement::DestoryTree(dialogs);
}

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}

static const OGUI::WindowContext NULL_WINDOW_CONTEXT = OGUI::WindowContext();
OGUI::WindowContext& OGUI::Context::GetOrRegisterWindowContext(const OGUI::WindowHandle window)
{
	for(auto&& ctx : windowContexts)
	{
		if(ctx.window == window)
			return ctx;
	}
	WindowContext newOne = WindowContext();
	newOne.window = window;
	newOne.X = 0;
	newOne.Y = 0;
	windowContexts.emplace_back(newOne);
	return windowContexts[windowContexts.size() - 1];
}

const OGUI::WindowContext& OGUI::Context::GetWindowContext(const OGUI::WindowHandle window) const
{
	for(const auto& ctx : windowContexts)
	{
		if(ctx.window == window)
			return ctx;
	}
	// warn("window context not found")
	return NULL_WINDOW_CONTEXT;
}

void OGUI::Context::OnKeyNavigation(OGUI::VisualElement* element, ENavDirection direction)
{
	auto nextNavTarget = element->FindNextNavTarget(direction);
	if(nextNavTarget)
	{
		SetFocus(nextNavTarget);
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
			preLostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			preLostKeyboardFocusEvent.newFocused = newKeyboardFocused;
			if(RouteEvent(_keyboardFocused, preLostKeyboardFocusEvent)) return false;
		}

		PreGotKeyboardFocusEvent preGotKeyboardFocusEvent;
		preGotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		preGotKeyboardFocusEvent.newFocused = newKeyboardFocused;
		if(RouteEvent(newKeyboardFocused, preGotKeyboardFocusEvent)) return false;
		auto oldKeyboardFocused = _keyboardFocused;
		_keyboardFocused = newKeyboardFocused;
		_currentActivateWindow = newWindow;

		if(oldKeyboardFocused)
		{
			LostKeyboardFocusEvent lostKeyboardFocusEvent;
			lostKeyboardFocusEvent.currentFocused = _keyboardFocused;
			lostKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
			RouteEvent(oldKeyboardFocused, lostKeyboardFocusEvent);
		}

		GotKeyboardFocusEvent gotKeyboardFocusEvent;
		gotKeyboardFocusEvent.currentFocused = _keyboardFocused;
		gotKeyboardFocusEvent.oldFocused = oldKeyboardFocused;
		RouteEvent(_keyboardFocused, gotKeyboardFocusEvent);
		return true;
		
	}
	return false;
}

bool OGUI::Context::SetFocus(OGUI::VisualElement* element)
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
			preGotFocusEvent.currentFocusedPath = &oldFocusedPath;
			preGotFocusEvent.newFocusedPath = &newFocusedPath;
			if(RouteEvent(newFocused, preGotFocusEvent)) return false;
		}
	}
	if(newKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		PreGotKeyboardFocusEvent preGotKeyboardFocusEvent;
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
			gotFocusEvent.currentFocusedPath = &newFocusedPath;
			gotFocusEvent.oldFocusedPath = &oldFocusedPath;
			RouteEvent(newFocused, gotFocusEvent);
		}
	}
	if(newKeyboardFocus && oldKeyboardFocus != newKeyboardFocus)
	{
		_keyboardFocused = newKeyboardFocus; //实际修改状态

		GotKeyboardFocusEvent gotKeyboardFocusEvent;
		gotKeyboardFocusEvent.currentFocused = newKeyboardFocus;
		gotKeyboardFocusEvent.oldFocused = oldKeyboardFocus;
		RouteEvent(newKeyboardFocus, gotKeyboardFocusEvent);
	}

	return true;
}