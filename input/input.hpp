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
	void UpdateInput(u32 Msg, u32 wParam);

	// Call this from your main loop AFTER inputs are processed.
	void ModerateInput();

	// Returns scroll distance this input cycle. Is reset to 0 each time ModerateInput is called.
	i32 GetScrollDistance();

	// Returns the key state of the VK keycode provided.
	KeyState_t GetKeyState(u32 KeyCode);
}