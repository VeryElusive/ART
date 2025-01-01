#include "input.hpp"

#ifdef _WIN32
#include <windows.h>
#else

#endif

ART::KeyState_t			KeyStates[256]		= {};
i32						Scroll				= 0;

i32						MouseX				= 0;
i32						MouseY				= 0;

i32						MouseDeltaX			= 0;
i32						MouseDeltaY			= 0;

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

		MouseDeltaX = MouseX - OldX;
		MouseDeltaY = MouseY - OldY;
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
	case WM_LBUTTONDOWN:
	{
		if(!KeyStates[VK_LBUTTON].Down)
		{
			KeyStates[VK_LBUTTON].Pressed = TRUE;
			KeyStates[VK_LBUTTON].Toggled = !KeyStates[VK_LBUTTON].Toggled;
		}
		KeyStates[VK_LBUTTON].Down = TRUE;
		break;
	}
	case WM_LBUTTONUP:
	{
		KeyStates[VK_LBUTTON].Down = FALSE;
		KeyStates[VK_LBUTTON].Released = TRUE;
		break;
	}
	case WM_RBUTTONDOWN:
	{
		if(!KeyStates[VK_RBUTTON].Down)
		{
			KeyStates[VK_RBUTTON].Pressed = TRUE;
			KeyStates[VK_RBUTTON].Toggled = !KeyStates[VK_RBUTTON].Toggled;
		}
		KeyStates[VK_RBUTTON].Down = TRUE;
		break;
	}
	case WM_RBUTTONUP:
	{
		KeyStates[VK_RBUTTON].Down = FALSE;
		KeyStates[VK_RBUTTON].Released = TRUE;
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