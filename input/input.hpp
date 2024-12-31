#pragma once
#include "../common/decl.hpp"

namespace ART
{
	struct KeyState_t
	{
		bool Pressed;
		bool Released;
		bool Down;
		bool Toggled;
	};

	// Call this from WndProc.
	void UpdateInput(u32 Msg, u32 wParam, u32 lParam);

	// Call this from your main loop AFTER inputs are processed.
	void ModerateInput();

	// Returns scroll distance this input cycle. Scroll distance is reset to 0 each time ModerateInput is called.
	i32 GetScrollDistance();

	// Sets X and Y to their axis' mouse position.
	void GetMousePosition(int& X, int& Y);

	// Sets X and Y to their axis' mouse delta. Mouse delta is reset to 0 each time ModerateInput is called.
	void GetMouseDelta(int &X, int &Y);

	// Returns the key state of the VK keycode provided.
	KeyState_t GetKeyState(u32 KeyCode);

	// Returns if the mouse is hovering over the provided position.
	bool IsMouseInBounds(int X, int Y, int W, int H);
}