#include "OpenGUI/Context.h"
#include <iostream>

void OGUI::Context::Update(int window, float dt)
{
	_deltaTime = dt;
}

void OGUI::Context::Render(int window)
{

}

bool OGUI::Context::OnMouseDown(int window, EMouseKey button, int32 x, int32 y)
{
	std::cout << "OnMouseDown: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseUp(int window, EMouseKey button, int32 x, int32 y)
{
	std::cout << "OnMouseUp: " << x << "," << y << std::endl;
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(int window, EMouseKey button, int32 x, int32 y)
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
