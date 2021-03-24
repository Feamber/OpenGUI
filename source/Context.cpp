#include "OpenGUI/Context.h"

void OGUI::Context::Update(int window, float dt)
{
	_deltaTime = dt;
}

void OGUI::Context::Render(int window)
{

}

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}
