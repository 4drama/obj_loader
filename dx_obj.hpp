#ifndef DX_OBJ__
#define DX_OBJ__

#include <Windows.h>
#include <d3dx9.h>

#include <vector>
#include <string>
#include <map>
#include <deque>

#include "mtl_indices.hpp"

struct vertex{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
	static const DWORD FVF;
};

struct triangle{
	std::size_t index[3];
};

struct material{
	D3DMATERIAL9 		material;
	IDirect3DTexture9*	diffuse_texture = 0;
	std::string 		diffuse_texture_full_path;
};

struct dx_obj{
	std::vector<vertex> 				vertexes;
	std::map<std::string, material> 	materials;
	std::deque<mtl_indices>				materials_indices;
	
	IDirect3DVertexBuffer9* VB = 0;
	std::size_t triangles_size;
		
	float translation[3];
	float rotation_x;
	float rotation_y;
	float rotation_z;
	float scaling[3];
	
	void set_translation(float x, float y, float z){
		this->translation[0] = x;
		this->translation[1] = y;
		this->translation[2] = z;
	}
	
	void add_translation(float x, float y, float z){
		this->translation[0] += x;
		this->translation[1] += y;
		this->translation[2] += z;
	}
	
	void set_scaling(float x, float y, float z){
		this->scaling[0] = x;
		this->scaling[1] = y;
		this->scaling[2] = z;
	}
};

D3DXMATRIX get_transform_matrix(dx_obj &obj);

dx_obj load_file(const std::string& path, const std::string& filename);
void load_to_device(IDirect3DDevice9* device, dx_obj &obj);

#endif