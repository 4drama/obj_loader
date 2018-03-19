#ifndef DX_INIT__
#define DX_INIT__

#include <Windows.h>
#include <d3dx9.h>
#include <string>

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
);

int dx_init(std::string app_name, HINSTANCE hInstance, int width, int height,
		bool windowed, D3DDEVTYPE device_type, IDirect3DDevice9** device);

#endif