#include "obj_loader.hpp"

#include "dx_app.hpp"

#include <iostream>
#include <cstring>

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd, 	_In_ UINT   uMsg,
	_In_ WPARAM wParam, _In_ LPARAM lParam){
	if(uMsg == WM_KEYDOWN){
		if(wParam == VK_ESCAPE)
			DestroyWindow(hwnd);
	} else if (uMsg == WM_DESTROY){
		PostQuitMessage(0);
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance,
		LPSTR cmdLine, int showCmd){
	
	dx_app App(hinstance);
	App.setup();
	App.msg_loop();
	
//	objl::obj_loader("test.txt");
	return 0;
}