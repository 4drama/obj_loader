#include "dx_app.hpp"
#include "dx_init.hpp"
#include "dx_obj.hpp"

#include <cstring>

dx_app::dx_app(HINSTANCE hInstance){
	dx_init("obj_loader", hInstance, 1400, 900, true, D3DDEVTYPE_HAL, &this->device);
}

dx_app::~dx_app(){
	this->cleanup();
}

void dx_app::setup(){
	load_file("test.txt");
}

void dx_app::cleanup(){
	if(vb != 0)
		vb->Release();
	
	if(ib != 0)
		ib->Release();
}

void dx_app::display(float time){
	
}

int dx_app::msg_loop(){
	MSG msg;
	std::memset(&msg, 0, sizeof(MSG));
	
	static float lastTime = (float)timeGetTime();
	
	while(msg.message != WM_QUIT){
		if(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}else{
			float currTime  = (float)timeGetTime();
			float timeDelta = (currTime - lastTime)*0.001f;
			
			this->display(timeDelta);
			
			lastTime = currTime;
		}
	}
	return msg.wParam;	
}