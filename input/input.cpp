#include "input.hpp"

#include <windows.h>

ART::KeyState_t	KeyStates[256];
i32				Scroll;

void ART::ModerateInput()
{
	for(Size_t i = 0; i < 256; i++)
	{
		KeyStates[i].Pressed = FALSE;
		KeyStates[i].Released = FALSE;
	}

	Scroll = 0;
}

void ART::UpdateInput(u32 Msg, u32 wParam)
{
	switch(Msg)
	{
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

i32 ART::GetScrollDistance()
{
	return Scroll;
}

ART::KeyState_t ART::GetKeyState(u32 KeyCode)
{
	return KeyStates[KeyCode];
}