/// TODO: change this into SDLs input
#include "window.h"
#include "SDL2/SDL_mouse.h"
#include "SDL2/SDL_video.h"
#include "glue.h"
#include "../utils.h"
#include "OpenGUI/Interface/Interfaces.h"
#include "OpenGUI/Context.h"
#include "OpenGUI/Core/OMath.h"
#include "OpenGUI/Event/PointerEvent.h"
#include "../appbase.h"

namespace OGUI
{
	struct WindowsInput : public InputInterface
	{
		virtual bool UseSystemGesture() override
		{
			return false;
		}
		virtual bool IsKeyDown(EKeyCode key_code) override
		{
			//SHORT pressed = GetKeyState((int)key_code);
			//if (pressed < 0)
			//{
			//	if (HI_BYTE(pressed) == 1)
			//	{
			//		printf("Key code: %d is pressed!", key_code);
			//	}
			//	else
			//		printf("Key code: %d is release!", key_code);
			//	return true;
			//}
			return false;
		}
		virtual bool IsKeyDown(EMouseKey key_code) override
		{
			return false;
		}
		virtual bool SetCursorPos(int32 x, int32 y) override
		{
			return ::SetCursorPos(x, y);
		}
		virtual bool SetCursor(EMouseCursor cursor) override
		{
			return false;
		}
		virtual bool GetCursorPos(int32& x, int32& y) override
		{
			SDL_GetMouseState(&x, &y);
			return false;
		}
		virtual bool IsKeyToggled(EMouseKey key_code) override
		{
			return false;
		}
		virtual void SetHighPrecisionMouseMode(WindowHandle window, bool Enable) override
		{
			OASSERT(window == 0);
			//not implemented
		}

		virtual OGUI::Vector2f GetDpiScale() override
		{
			HDC hdc = ::GetDC(NULL); // get the device context to get the dpi info
			OGUI::Vector2f ret = {};
			const float dpi = 96.0f;
			if (hdc)
			{
				ret.X = ::GetDeviceCaps(hdc, LOGPIXELSX) / dpi;
				ret.Y = ::GetDeviceCaps(hdc, LOGPIXELSY) / dpi;
				::ReleaseDC(NULL, hdc);
			}
			else
			{
#if(WINVER >= 0x0605)
				float systemDpi = ::GetDpiForSystem() / 96.0f;
				ret = { systemDpi, systemDpi };
#else
				ret = { 1.0f, 1.0f };
#endif
			}
			return ret;
		}

		void CapturePointer(WindowHandle window, bool enabled) override
		{
			auto winImpl = (AppWindow*)window;
			SDL_CaptureMouse((SDL_bool)enabled);
			//SDL_SetWindowGrab(winImpl->window, (SDL_bool)enabled);
		}
	};
}

void InstallInput()
{
	auto& ctx = OGUI::Context::Get();
	BuildSDLMap();
	ctx.inputImpl = std::make_unique<OGUI::WindowsInput>();
}