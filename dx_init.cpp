#include "dx_init.hpp"

#include <cstring>

int dx_init(std::string app_name, HINSTANCE hInstance, int width, int height,
		bool windowed, D3DDEVTYPE device_type, IDirect3DDevice9** device){
	
	WNDCLASS wc;
	std::memset(&wc, 0, sizeof(WNDCLASS));
	
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = (WNDPROC)WindowProc;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wc.lpszClassName = app_name.c_str();
	
	if(!RegisterClass(&wc)){
		MessageBox(0, "RegisterClass() - FAILED", 0, 0);
		return -1;
	}
}