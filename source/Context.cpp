#include "OpenGUI/Context.h"

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

void OGUI::Context::Update(int window, float dt)
{
	_deltaTime = dt;
}

void OGUI::Context::Render(int window)
{

}

bool OGUI::Context::OnMouseDown(int window, EMouseKey button, int32 x, int32 y)
{

	return false;
}

bool OGUI::Context::OnMouseUp(int window, EMouseKey button, int32 x, int32 y)
{
	return false;
}

bool OGUI::Context::OnMouseDoubleClick(int window, EMouseKey button, int32 x, int32 y)
{
	return false;
}

bool OGUI::Context::OnMouseMove(bool relative, int32 x, int32 y)
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
