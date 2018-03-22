#include "dx_init.hpp"

#include <cstring>

namespace{
	int win_init_(std::string app_name, HINSTANCE hInstance,
			int width, int height, HWND *hwnd);
}

int dx_init(std::string app_name, HINSTANCE hInstance, int width, int height,
		bool windowed, D3DDEVTYPE device_type, IDirect3DDevice9** device){
	
	HWND hwnd = 0;
	if(win_init_(app_name, hInstance, width, height, &hwnd) < 0)
		return -1;
	
	HRESULT hr = 0;
	IDirect3D9* d3d9 = 0;
    d3d9 = Direct3DCreate9(D3D_SDK_VERSION);
	if( !d3d9 ){
		MessageBox(0, "Direct3DCreate9() - FAILED", 0, 0);
		return -1;
	}
	
	D3DCAPS9 caps;
	d3d9->GetDeviceCaps(D3DADAPTER_DEFAULT, device_type, &caps);
	
	int vp = 0;
	if( caps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT )
		vp = D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		vp = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
	
	D3DPRESENT_PARAMETERS d3dpp;
	d3dpp.BackBufferWidth            = width;
	d3dpp.BackBufferHeight           = height;
	d3dpp.BackBufferFormat           = D3DFMT_A8R8G8B8;
	d3dpp.BackBufferCount            = 1;
	d3dpp.MultiSampleType            = D3DMULTISAMPLE_NONE;
	d3dpp.MultiSampleQuality         = 0;
	d3dpp.SwapEffect                 = D3DSWAPEFFECT_DISCARD; 
	d3dpp.hDeviceWindow              = hwnd;
	d3dpp.Windowed                   = windowed;
	d3dpp.EnableAutoDepthStencil     = true; 
	d3dpp.AutoDepthStencilFormat     = D3DFMT_D24S8;
	d3dpp.Flags                      = 0;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;
	
	hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, device_type, hwnd,
		vp, &d3dpp, device);

	if(FAILED(hr)){
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		
		hr = d3d9->CreateDevice(D3DADAPTER_DEFAULT, device_type, hwnd,
			vp, &d3dpp, device);

		if(FAILED(hr)){
			d3d9->Release();
			MessageBox(0, "CreateDevice() - FAILED", 0, 0);
			return false;
		}
	}

	d3d9->Release();
	
	return 0;
}

int MsgLoop(bool (*ptr_display)(float timeDelta)){
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
			
			ptr_display(timeDelta);
			
			lastTime = currTime;
		}
	}
	return msg.wParam;
}

namespace{
	int win_init_(std::string app_name, HINSTANCE hInstance, int width, int height,
			HWND *hwnd){
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
		
		*hwnd = CreateWindow(app_name.c_str(), app_name.c_str(), 
			WS_EX_TOPMOST,
			0, GetSystemMetrics(SM_CYMAXTRACK) - GetSystemMetrics(SM_CYMAXIMIZED),
			width, height, 0, 0, hInstance, 0); 
		
		if( !(*hwnd) ){
			MessageBox(0, "CreateWindow() - FAILED", 0, 0);
			return -1;
		}
		
		ShowWindow(*hwnd, SW_SHOW);
		UpdateWindow(*hwnd);
	}
}