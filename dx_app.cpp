#include "dx_app.hpp"
#include "dx_init.hpp"

#include <cstring>

#include <iostream>


namespace{
	 D3DLIGHT9 create_light(D3DXVECTOR3 pos){
		D3DLIGHT9 dir;
		ZeroMemory(&dir, sizeof(dir));
		dir.Type = D3DLIGHT_POINT;
		dir.Diffuse = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255) * 0.3f;
		dir.Specular = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255) * 0.6f;
		dir.Ambient = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255) * 0.6f;
		dir.Range = 10;
		dir.Attenuation0 = 0.4f;
		dir.Attenuation1 = 0.2f;
		dir.Attenuation2 = 0.05f;
		dir.Position = pos;
		return dir;
	 }
}

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
	
	D3DXVECTOR3 cam_pos(3.0f, 0.0f, 2.0f);
	
	this->main_cam.position = cam_pos;
	this->main_cam.target = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	this->main_cam.up = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
	
	D3DLIGHT9 light = create_light(cam_pos);
	this->add_light(light, nullptr);
	
	D3DMATERIAL9 mtrl;
		mtrl.Ambient = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255);
		mtrl.Diffuse = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255);
		mtrl.Specular = (D3DXCOLOR)D3DCOLOR_XRGB(255, 255, 255);
		mtrl.Emissive = (D3DXCOLOR)D3DCOLOR_XRGB(0, 0, 0);
		mtrl.Power = 2.0f;
	this->device->SetMaterial(&mtrl);
	
	this->device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	this->device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	this->device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_POINT);
		
	this->device->SetRenderState(D3DRS_NORMALIZENORMALS, true);
	this->device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	this->device->SetRenderState(D3DRS_LIGHTING, true);
	
	this->set_perspective();
	this->set_view(this->main_cam);
	
	this->obj = load_file("./challenger/", "CHALLENGER71.obj");
	load_to_device(this->device, this->obj);
	
	D3DXMatrixScaling(&this->obj.scaling, 1, 1, 1);
	D3DXMatrixTranslation(&this->obj.translation, 0, 0, 0);
	D3DXMatrixRotationX(&this->obj.rotation_x, 0);
	D3DXMatrixRotationY(&this->obj.rotation_y, 0);
	D3DXMatrixRotationZ(&this->obj.rotation_z, 0);
}

void dx_app::cleanup(){
	
}

void dx_app::display(float time){
	if( this->device ){
		this->device->Clear(0, 0,
			D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER,
			0x000000ff, 1.0f, 0);
		this->device->BeginScene();
		
		this->set_view(this->main_cam);
		
/*		this->device->SetStreamSource(0, this->obj.VB, 0, sizeof(vertex));
		this->device->SetFVF(vertex::FVF);

		D3DXMATRIX transform = get_transform_matrix(this->obj);
		this->device->SetTransform(D3DTS_WORLD, &transform);
		
		this->device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, this->obj.triangles_size);*/
		
		this->render();
		
		this->device->EndScene();
		this->device->Present(0, 0, 0, 0);
	}
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

std::size_t dx_app::add_light(D3DLIGHT9 light, D3DLIGHT9 **light_ptr){
	D3DLIGHT9 *new_light_ptr;
	std::size_t i = this->lights.add(light, &new_light_ptr);
	this->device->SetLight(i, new_light_ptr);
	this->device->LightEnable(i, true);
	
	if(light_ptr != nullptr)
		*light_ptr = new_light_ptr;
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

void dx_app::render(){
	
	this->device->SetStreamSource(0, this->obj.VB, 0, sizeof(vertex));
	this->device->SetFVF(vertex::FVF);

	D3DXMATRIX transform = get_transform_matrix(this->obj);
	this->device->SetTransform(D3DTS_WORLD, &transform);
	
//==================REMOVE===============	
	IDirect3DTexture9* Tex = 0;
//=======================================
	
	for(auto& curr_mtl_index : obj.materials_indices){
		
		this->device->SetMaterial(&obj.materials[curr_mtl_index.name].material);

//==================REMOVE===============
		if(obj.materials[curr_mtl_index.name].diffuse_texture_full_path.size() != 0){
			D3DXCreateTextureFromFile(
			this->device,
			obj.materials[curr_mtl_index.name].diffuse_texture_full_path.c_str(),
			&Tex);		
			this->device->SetTexture(0, Tex);
		} else
			this->device->SetTexture(0, NULL);
//=======================================
		
		this->device->DrawPrimitive(
			D3DPT_TRIANGLELIST, curr_mtl_index.begin, curr_mtl_index.end);
		
	}
}