#include "OpenGUI/Context.h"
#include <iostream>
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/VisualWindow.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/IOThread.h"
#include "OpenGUI/Managers/RenderTextureManager.h"

void OGUI::Context::Initialize(
	InputInterface* I, SystemInterface* S,
	RenderInterface* R, FileInterface* F,
	BitmapParserInterface* B)
{
	if(initialized)
		assert(0 && "already initialized!");
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
		element->DrawBackgroundPrimitive(ctx);
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
			if(auto picked = PickRecursive(child.get(), point))
				return picked;
		auto invTransform = math::inverse(element->_worldTransform);
		Vector4f dummy = {point.X, point.Y, 0.f, 1.f};
		const Vector4f result = math::multiply(dummy, invTransform);
		Vector2f localPoint = {result.X, result.Y};

		//std::cout << "OnMouseDown: " << localPoint.X << "," << localPoint.Y << std::endl;
		//std::cout << "Name: " << element->_name << std::endl;
		//std::cout << "Rect: " << element->GetRect().min.X << element->GetRect().min.Y << std::endl;
		if (element->Intersect(localPoint))
			return element;
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
	auto root = desktops.get();
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
	auto root = desktops.get();
	PrimitiveDraw::DrawContext ctx;
	const auto& wctx = GetWindowContext(window);
	ctx.resolution = Vector2f(wctx.X, wctx.Y);
	root->Traverse([&](VisualElement* next) { RenderRec(next, ctx); });
	renderImpl->RenderPrimitives(ctx.prims);
}

bool OGUI::Context::OnMouseDown(const WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	auto root = desktops.get();
	if (!root)
		return false;
	PointerDownEvent event;
	//todo: what is mouse up is missing?
	pointerDownCount++;
	event.pointerType = "mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;

	event.position = Vector2f(x, y); // screen space
	inputImpl->ScreenToClient(window, x, y); // client space

	auto dpiScale = inputImpl->GetDpiScale();
	x /= dpiScale.X;
	y /= dpiScale.Y;

	const auto& wctx = GetWindowContext(window);
	const float width = wctx.X;
	const float height = wctx.Y;

	auto point = Vector2f(x, height - y) - Vector2f(width, height) / 2; // center of the window

	auto picked = PickRecursive(root, point);
	if (picked)
	{
		if (picked != currentFocus.lock().get())
		{
			picked->_pseudoMask |= (int)PseudoStates::Focus;
			if (auto currF = currentFocus.lock().get())
			{
				currF->_pseudoMask &= ~(int)PseudoStates::Focus;
			}
		}
		currentFocus = picked->shared_from_this();
		RouteEvent(picked, event);
	}
	return false;
}

bool OGUI::Context::OnMouseUp(const WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	pointerDownCount--;
	//std::cout << "OnMouseUp: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(const WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	//std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseMove(const WindowHandle window, bool relative, int32 x, int32 y)
{
	//auto root = desktops.get();
	//if (!root)
	//	return false;

	//std::cout << "OnMouseMove: " << x << "," << y << std::endl;

	//static Vector2f prevPointerPos;
	//prevPointerPos = { (float)x, (float)y };

	//PointerMoveEvent moveEvent;
	//moveEvent.position = { (float)x, (float)y };
	//moveEvent.pointerType = "mouse";
	//moveEvent.deltaPosition = { (float)x - prevPointerPos.X, (float)y - prevPointerPos.Y };

	//auto dpiScale = inputImpl->GetDpiScale();
	//x /= dpiScale.X;
	//y /= dpiScale.Y;

	//const auto& wctx = GetWindowContext(window);
	//const float width = wctx.X;
	//const float height = wctx.Y;

	//auto point = Vector2f(x, height - y) - Vector2f(width, height) / 2; // center of the window

	//auto picked = PickRecursive(root, point);
	//if (picked)
	//{
	//	if (picked != currentFocus.lock().get())
	//	{
	//		picked->_pseudoMask |= (int)PseudoStates::Hover;
	//		if (auto currF = currentFocus.lock().get())
	//		{
	//			currF->_pseudoMask &= ~(int)PseudoStates::Hover;
	//		}
	//	}
	//	currentFocus = picked->shared_from_this();
	//	RouteEvent(picked, moveEvent);
	//}

	//prevPointerPos = { (float)x, (float)y };
	return false;
}

bool OGUI::Context::OnMouseMoveHP(bool relative, float x, float y)
{
	return false;
}

bool OGUI::Context::OnMouseWheel(float delta)
{
	return false;
}

OGUI::Context::Context()
{
	ioThread = std::make_unique<IOThread>();
	textureManager = std::make_unique<RenderTextureManager>();
}

OGUI::Context::~Context()
{
	desktops.reset();
	dialogs.reset();
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

