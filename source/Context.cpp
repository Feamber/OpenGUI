#include "OpenGUI/Context.h"
#include <iostream>
#include "OpenGUI/Core/AsyncFile.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "OpenGUI/Event/EventRouter.h"
#include "OpenGUI/Window/VisualWindow.h"
#include "OpenGUI/Core/PrimitiveDraw.h"
#include "OpenGUI/Core/IOThread.h"

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
		element->UpdateWorldTransform();
		element->Traverse([&](VisualElement* next) { TransformRec(next); });
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

		std::cout << "OnMouseDown: " << localPoint.X << "," << localPoint.Y << std::endl;
		std::cout << "Name: " << element->_name << std::endl;
		std::cout << "Rect: " << element->GetRect().min.X << element->GetRect().min.Y << std::endl;
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
	animSystem.Update(root);
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
	//todo: what is mouse up is missing?
	pointerDownCount++;
	event.pointerType = "mouse";
	event.button = button;
	event.isPrimary = pointerDownCount == 1;
	event.gestureType = EGestureEvent::None;

	event.position = Vector2f(x, y); // screen space
	inputImpl->ScreenToClient(window, x, y); // client space

	auto dpiScale = inputImpl->GetDpiScale();
	float width = 800.0f * dpiScale.X;
	float height = 450.0f * dpiScale.Y;
	auto point = Vector2f(x, height - y) - Vector2f(width, height) / 2; // center of the window

	auto picked = PickRecursive(root, point);
	std::cout << std::endl;
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

bool OGUI::Context::OnMouseUp(WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	pointerDownCount--;
	//std::cout << "OnMouseUp: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(WindowHandle window, EMouseKey button, int32 x, int32 y)
{
	//std::cout << "OnMouseDoubleClick: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseMove(bool relative, int32 x, int32 y)
{
	//std::cout << "OnMouseMove: " << x << "," << y << std::endl;
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
