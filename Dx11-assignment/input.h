#pragma once

#include <dinput.h>

class input
{
public:
	input(HINSTANCE* hInst, HWND* hWnd);
	~input();

	HRESULT initialise();
	void ReadInputStates();
	bool IsKeyPressed(unsigned char input);
	float GetMouseX();
	float GetMouseY();
	bool IsMouseButtonPressed(int input);
private:

	IDirectInput8* m_direct_input;
	IDirectInputDevice8* m_keyboard_device;
	IDirectInputDevice8* m_mouse_device;
	DIMOUSESTATE m_mouse_state;
	unsigned char m_keyboard_keys_state[256];

	HINSTANCE* m_hInst;
	HWND*		m_hWnd;
	float sensitivity;

};

