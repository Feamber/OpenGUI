#include "OpenGUI/Context.h"

void OGUI::Context::Update(int window, float dt)
{
	deltaTime = dt;
}

OGUI::Context& OGUI::Context::Get()
{
	static Context ctx;
	return ctx;
}
