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
	D3DMATERIAL9 	material;
	std::string 	diffuse_texture_full_path;
};

struct dx_obj{
	std::vector<vertex> 				vertexes;
	std::map<std::string, material> 	materials;
	std::deque<mtl_indices>				materials_indices;
	
	IDirect3DVertexBuffer9* VB = 0;
	std::size_t triangles_size;
	
	D3DXMATRIX translation;
	D3DXMATRIX rotation_x;
	D3DXMATRIX rotation_y;
	D3DXMATRIX rotation_z;
	D3DXMATRIX scaling;
};

D3DXMATRIX get_transform_matrix(dx_obj &obj);

dx_obj load_file(const std::string& path, const std::string& filename);
void load_to_device(IDirect3DDevice9* device, dx_obj &obj);

#endif