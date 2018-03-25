#ifndef DX_APP__
#define DX_APP__

#include <Windows.h>
#include <d3dx9.h>

#include "index_list.hpp"

struct camera{
	D3DXVECTOR3 position;
	D3DXVECTOR3 target;
	D3DXVECTOR3 up;
};

class dx_app{
public:
	dx_app() = delete;	
	dx_app(const dx_app&) = delete;
	
	dx_app(HINSTANCE hInstance);
	~dx_app();
	
	void setup();
	int msg_loop();
private:
	IDirect3DDevice9* device;
	float width, height;
	
	camera main_cam;
	index_list<D3DLIGHT9> lights;
	
	void display(float time);
	void cleanup();
	void set_view(const camera &cam);
	void set_perspective();
	std::size_t add_light(D3DLIGHT9 light);
	void del_light(std::size_t index);
};

#endif