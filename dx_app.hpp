#ifndef DX_APP__
#define DX_APP__

#include <Windows.h>
#include <d3dx9.h>

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
	
	void display(float time);
	void cleanup();
};

#endif