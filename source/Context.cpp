#include "OpenGUI/Context.h"
#include <iostream>
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Window/VisualWindow.h"
#include "OpenGUI/Core/PrimitiveDraw.h"

void OGUI::Context::Initialize(
	InputInterface* I, SystemInterface* S,
	RenderInterface* R, FileInterface* F)
{
	if(initialized)
		assert(0 && "already initialized!");
	inputImpl.reset(I);
	systemImpl.reset(S);
	renderImpl.reset(R);
	fileImpl.reset(F);
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
		element->UpdateWorldTransform();
		element->Traverse([&](VisualElement* next) { TransformRec(next); });
	}
	VisualElement* PickRecursive(VisualElement* element, Vector2f point)
	{
		for (auto& child : element->_children)
			if(auto picked = PickRecursive(child.get(), point))
				return picked;
		auto invTransform = math::inverse(element->_worldTransform);
		Vector4f dummy = {point.X, point.Y, 0.f, 0.f};
		dummy = math::multiply(dummy, invTransform);
		Vector2f localPoint = {dummy.X, dummy.Y};
		if (element->Intersect(localPoint))
			return element;
		else return nullptr;
	}
}

void OGUI::Context::Update(WindowHandle window, float dt)
{
	auto root = desktops.get();
	_deltaTime = dt;
	styleSystem.Update(root);
	root->CalculateLayout();
	TransformRec(root);
}

void OGUI::Context::Render(WindowHandle window)
{
	auto root = desktops.get();
	PrimitiveDraw::DrawContext ctx;
	ctx.resolution = Vector2f(800.f, 450.f);
	RenderRec(root, ctx);
	renderImpl->RenderPrimitives(ctx.prims);
}

bool OGUI::Context::OnMouseDown(WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	auto root = desktops.get();
	if (!root)
		return false;
	PointerDownEvent event;
	event.pointerType = "mouse";
	event.buttonPrimary = button;
	event.gestureType = EGestureEvent::None;
	event.position = Vector2f(x, y);
	inputImpl->ScreenToClient(window, x, y);
	auto picked = PickRecursive(root, Vector2f(x, y));
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

	std::cout << "OnMouseDown: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseUp(WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	std::cout << "OnMouseUp: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseMove(bool relative, int32 x, int32 y)
{
	std::cout << "OnMouseMove: " << x << "," << y << std::endl;
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

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}
