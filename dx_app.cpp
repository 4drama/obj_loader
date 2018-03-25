#include "dx_app.hpp"
#include "dx_init.hpp"
#include "dx_obj.hpp"

#include <cstring>

dx_app::dx_app(HINSTANCE hInstance){
	
	this->width = 1400;
	this->height = 900;
	
	dx_init("obj_loader", hInstance, this->width, this->height,
		true, D3DDEVTYPE_HAL, &this->device);
}

dx_app::~dx_app(){
	this->cleanup();
}

void dx_app::setup(){
	
	this->main_cam.position = D3DXVECTOR3(0.0f, 0.0f, -5.0f);
	this->main_cam.target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->main_cam.up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	
	this->device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	this->device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	this->device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		
	this->device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->device->SetRenderState(D3DRS_LIGHTING, true);
	
	this->set_perspective();
	this->set_view(this->main_cam);
	
	load_file("test.txt");
}

void dx_app::cleanup(){
	
}

void dx_app::display(float time){
	
}

void dx_app::set_view(const camera &cam){
	D3DXMATRIX V;
	D3DXMatrixLookAtLH(&V, &cam.position, &cam.target, &cam.up);
	this->device->SetTransform(D3DTS_VIEW, &V);
}

void dx_app::set_perspective(){
	D3DXMATRIX proj;
	D3DXMatrixPerspectiveFovLH(
		&proj,
		D3DX_PI * 0.5f, // 90 градусов
		(float)this->width / (float)this->height,
		1.0f,
		1000.0f);
	this->device->SetTransform(D3DTS_PROJECTION, &proj);
}

std::size_t dx_app::add_light(D3DLIGHT9 light){
	D3DLIGHT9 *light_ptr;
	std::size_t i = this->lights.add(light, &light_ptr);
	
	this->device->SetLight(i, light_ptr);
	this->device->LightEnable(i, true);
	return i;
}

void dx_app::del_light(std::size_t index){
	this->lights.remove(index);
	
	this->device->LightEnable(index, false);
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