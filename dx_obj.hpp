#ifndef DX_OBJ__
#define DX_OBJ__

#include <Windows.h>
#include <d3dx9.h>

#include <vector>
#include <string>

struct vertex{
	float x, y, z;
	float nx, ny, nz;
	float u, v;
	static const DWORD FVF;
};

struct triangle{
	std::size_t index[3];
};

struct dx_obj{
	std::vector<vertex> vertexes;
	std::vector<triangle> indices;
};

dx_obj load_file(const std::string& filename);

#endif