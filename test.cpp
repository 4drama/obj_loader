#include "obj_loader.hpp"

#include "dx_init.hpp"

#include <iostream>

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd, 	_In_ UINT   uMsg,
	_In_ WPARAM wParam, _In_ LPARAM lParam){
	if(uMsg == WM_KEYDOWN){
		if(wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

bool display(float time){
	
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance,
		LPSTR cmdLine, int showCmd){
	
	IDirect3DDevice9* Device = 0;	
	dx_init("obj_loader", hinstance, 1400, 900, true, D3DDEVTYPE_HAL, &Device);
	
	MsgLoop(display);
	
	objl::obj_loader("test.txt");
	return 0;
}