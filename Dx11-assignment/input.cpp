#include "input.h"


input::input(HINSTANCE * hInst, HWND* hWnd)
{
	m_hInst = hInst;
	m_hWnd = hWnd;
	sensitivity = 0.2f;
}

input::~input()
{
	m_mouse_device->Unacquire();
	m_mouse_device->Release();
	m_keyboard_device->Unacquire();
	m_keyboard_device->Release();
}

HRESULT input::initialise()
{
	HRESULT hr = false;
	ZeroMemory(m_keyboard_keys_state, sizeof(m_keyboard_keys_state));

	hr = DirectInput8Create(*m_hInst, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_direct_input, NULL);
	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysKeyboard, &m_keyboard_device, NULL);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return hr;

	hr = m_keyboard_device->Acquire();
	if (FAILED(hr)) return hr;

	hr = m_direct_input->CreateDevice(GUID_SysMouse, &m_mouse_device, NULL);
	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return hr;

	hr = m_mouse_device->SetCooperativeLevel(*m_hWnd, DISCL_FOREGROUND | DISCL_EXCLUSIVE);

	hr = m_mouse_device->Acquire();
	if (FAILED(hr)) return hr;

	return S_OK;
}

void input::ReadInputStates()
{
	HRESULT hr;
	hr = m_keyboard_device->GetDeviceState(sizeof(m_keyboard_keys_state), (LPVOID)&m_keyboard_keys_state);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_keyboard_device->Acquire();
		}
	}
	hr = m_mouse_device->GetDeviceState(sizeof(m_mouse_state), (LPVOID)&m_mouse_state);
	if (FAILED(hr))
	{
		if ((hr == DIERR_INPUTLOST) || (hr == DIERR_NOTACQUIRED))
		{
			m_mouse_device->Acquire();
		}
	}
}

bool input::IsKeyPressed(unsigned char DI_keycode)
{
	return m_keyboard_keys_state[DI_keycode];
}

float input::GetMouseX() // get left and right
{
	return m_mouse_state.lX * sensitivity;
}

float input::GetMouseY() // get up and down
{
	return -1 *(m_mouse_state.lY * sensitivity);
}

bool input::IsMouseButtonPressed(int input)
{
	return m_mouse_state.rgbButtons[input];
}
