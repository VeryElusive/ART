#include "input.hpp"

#ifdef _WIN32
#include <windows.h>
#else

#endif

ART::KeyState_t			KeyStates[256] = {};
i32						Scroll = 0;

i32						MouseX = 0;
i32						MouseY = 0;

i32						MouseDeltaX = 0;
i32						MouseDeltaY = 0;

void ART::ModerateInput()
{
#ifdef _WIN32
	for(Size_t i = 0; i < 256; i++)
	{
		KeyStates[i].Pressed = FALSE;
		KeyStates[i].Released = FALSE;
	}

	Scroll = 0;

	MouseDeltaX = 0;
	MouseDeltaY = 0;
#else

#endif
}

#ifdef _WIN32
void ART::UpdateInput(u32 Msg, u32 wParam, u32 lParam)
{
	switch(Msg)
	{
	case WM_MOUSEMOVE:
	{
		const int OldX = MouseX;
		const int OldY = MouseY;

		MouseX = LOWORD(lParam);
		MouseY = HIWORD(lParam);

		MouseDeltaX += MouseX - OldX;
		MouseDeltaY += MouseY - OldY;
		break;
	}
	case WM_KEYDOWN:
	{
		if(wParam < 256 && wParam >= 0)
		{
			if(!KeyStates[wParam].Down)
			{
				KeyStates[wParam].Pressed = TRUE;
				KeyStates[wParam].Toggled = !KeyStates[wParam].Toggled;
			}
			KeyStates[wParam].Down = TRUE;
		}

		break;
	}
	case WM_KEYUP:
	{
		if(wParam < 256 && wParam >= 0)
		{
			KeyStates[wParam].Down = FALSE;
			KeyStates[wParam].Released = TRUE;
		}

		break;
	}
	case WM_SYSKEYDOWN:
	{
		if(wParam < 256 && wParam >= 0)
		{
			if(!KeyStates[wParam].Down)
			{
				KeyStates[wParam].Pressed = TRUE;
				KeyStates[wParam].Toggled = !KeyStates[wParam].Toggled;
			}
			KeyStates[wParam].Down = TRUE;
		}

		break;
	}
	case WM_SYSKEYUP:
	{
		if(wParam < 256 && wParam >= 0)
		{
			KeyStates[wParam].Down = FALSE;
			KeyStates[wParam].Released = TRUE;
		}

		break;
	}
	case WM_LBUTTONDOWN:
	{
		if(!KeyStates[ART::LBUTTON].Down)
		{
			KeyStates[ART::LBUTTON].Pressed = TRUE;
			KeyStates[ART::LBUTTON].Toggled = !KeyStates[ART::LBUTTON].Toggled;
		}
		KeyStates[ART::LBUTTON].Down = TRUE;
		break;
	}
	case WM_LBUTTONUP:
	{
		KeyStates[ART::LBUTTON].Down = FALSE;
		KeyStates[ART::LBUTTON].Released = TRUE;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if(!KeyStates[ART::RBUTTON].Down)
		{
			KeyStates[ART::RBUTTON].Pressed = TRUE;
			KeyStates[ART::RBUTTON].Toggled = !KeyStates[ART::RBUTTON].Toggled;
		}
		KeyStates[ART::RBUTTON].Down = TRUE;
		break;
	}
	case WM_RBUTTONUP:
	{
		KeyStates[ART::RBUTTON].Down = FALSE;
		KeyStates[ART::RBUTTON].Released = TRUE;
		break;
	}
	case WM_MBUTTONDOWN:
	{
		if(!KeyStates[ART::MBUTTON].Down)
		{
			KeyStates[ART::MBUTTON].Pressed = TRUE;
			KeyStates[ART::MBUTTON].Toggled = !KeyStates[ART::MBUTTON].Toggled;
		}
		KeyStates[ART::MBUTTON].Down = TRUE;
		break;
	}
	case WM_MBUTTONUP:
	{
		KeyStates[ART::MBUTTON].Down = FALSE;
		KeyStates[ART::MBUTTON].Released = TRUE;
		break;
	}
#undef XBUTTON1
#undef XBUTTON2
	case WM_XBUTTONDOWN:
	{
		WORD button = GET_XBUTTON_WPARAM(wParam);
		if(button == 0x0001/*XBUTTON1*/)
		{
			if(!KeyStates[ART::XBUTTON1].Down)
			{
				KeyStates[ART::XBUTTON1].Pressed = TRUE;
				KeyStates[ART::XBUTTON1].Toggled = !KeyStates[ART::XBUTTON1].Toggled;
			}
			KeyStates[ART::XBUTTON1].Down = TRUE;
		}
		else if(button == 0x0002/*XBUTTON2*/)
		{
			if(!KeyStates[ART::XBUTTON2].Down)
			{
				KeyStates[ART::XBUTTON2].Pressed = TRUE;
				KeyStates[ART::XBUTTON2].Toggled = !KeyStates[ART::XBUTTON2].Toggled;
			}
			KeyStates[ART::XBUTTON2].Down = TRUE;
		}
		break;
	}
	case WM_XBUTTONUP:
	{
		WORD button = GET_XBUTTON_WPARAM(wParam);
		if(button == 0x0001/*XBUTTON1*/)
		{
			KeyStates[ART::XBUTTON1].Down = FALSE;
			KeyStates[ART::XBUTTON1].Released = TRUE;
		}
		else if(button == 0x0002/*XBUTTON2*/)
		{
			KeyStates[ART::XBUTTON2].Down = FALSE;
			KeyStates[ART::XBUTTON2].Released = TRUE;
		}
		break;
	}
	case WM_MOUSEWHEEL:
	case WM_MOUSEHWHEEL:
	{
		Scroll += (i32)(float)GET_WHEEL_DELTA_WPARAM(wParam) / (float)WHEEL_DELTA;
		break;
	}
	default:
		break;
	}
}
#else

#endif

i32 ART::GetScrollDistance()
{
	return Scroll;
}

ART::KeyState_t ART::GetKeyState(u32 KeyCode)
{
	if(KeyCode > 255 || KeyCode < 0)
	{
		return {};
	}

	return KeyStates[KeyCode];
}

void ART::GetMousePosition(int &X, int &Y)
{
	X = MouseX;
	Y = MouseY;
}

void ART::GetMouseDelta(int &X, int &Y)
{
	X = MouseDeltaX;
	Y = MouseDeltaY;
}

bool ART::IsMouseInBounds(int X, int Y, int W, int H)
{
	return (MouseX >= X && MouseX <= (X + W)
		&& MouseY >= Y && MouseY <= (Y + H));
}