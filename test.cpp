#include "obj_loader.hpp"

#include "dx_init.hpp"

LRESULT CALLBACK WindowProc(
	_In_ HWND   hwnd,
	_In_ UINT   uMsg,
	_In_ WPARAM wParam,
	_In_ LPARAM lParam
){
	
}

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE prevInstance,
		LPSTR cmdLine, int showCmd){
	
	
	objl::obj_loader("test.txt");
	return 0;
}