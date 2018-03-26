#include "dx_obj.hpp"

#include "obj_loader.hpp"

#include <iostream>

const DWORD vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

namespace {
	void add_vertex(const objl::vertex &input_obj_vertex, vertex &output_dx_vertex){
		output_dx_vertex.x = input_obj_vertex.x;
		output_dx_vertex.y = input_obj_vertex.y;
		output_dx_vertex.z = input_obj_vertex.z;
	}
	
	void add_normal(const objl::normal &input_obj_normal, vertex &output_dx_vertex){
		output_dx_vertex.nx = input_obj_normal.xn;
		output_dx_vertex.ny = input_obj_normal.yn;
		output_dx_vertex.nz = input_obj_normal.zn;
	}
	
	void add_texture_uv(const objl::texture_uv &texture_uv_obj_vertex, 
			vertex &output_dx_vertex){
		output_dx_vertex.u = texture_uv_obj_vertex.u;
		output_dx_vertex.v = texture_uv_obj_vertex.v;
	}
	
	vertex create_vertex(	const objl::vertex &input_obj_vertex,
							const objl::normal &input_obj_normal,
							const objl::texture_uv &texture_uv_obj_vertex){
		vertex res;
		add_vertex(input_obj_vertex, res);
		add_normal(input_obj_normal, res);
		add_texture_uv(texture_uv_obj_vertex, res);
		return res;
	}
	
	triangle next_triangle(std::size_t first_index){
		triangle res;
		res.index[0] = first_index;
		res.index[1] = first_index + 1;
		res.index[2] = first_index + 2;
		return res;
	}
}

dx_obj load_file(const std::string& filename){	
	objl::object obj = objl::obj_loader(filename);
	dx_obj result;
	
	for(auto &current_triangle : obj.triangles){
		result.indices.push_back(next_triangle(result.vertexes.size()));
		
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_first],
				obj.normals[current_triangle.normal_first],
				obj.textures_uv[current_triangle.uv_first]));
				
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_second],
				obj.normals[current_triangle.normal_second],
				obj.textures_uv[current_triangle.uv_second]));
				
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_third],
				obj.normals[current_triangle.normal_third],
				obj.textures_uv[current_triangle.uv_third]));		
	}	
	return result;
}

void load_to_device(IDirect3DDevice9* device, dx_obj &obj){
	
	device->CreateVertexBuffer(
		obj.vertexes.size() * sizeof(vertex),
		D3DUSAGE_WRITEONLY,
		vertex::FVF,
		D3DPOOL_MANAGED,
		&obj.VB,
		0);
	vertex* vertices_ptr;
	
	device->CreateIndexBuffer(
		obj.vertexes.size() * sizeof(WORD),
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&obj.IB,
		0);
	WORD* indices_ptr = 0;
	
	obj.IB->Lock(0, 0, (void**)&indices_ptr, 0);
	obj.VB->Lock(0, 0, (void**)&vertices_ptr, 0);
	
	for(int i = 0; i != obj.vertexes.size(); i++){
		vertices_ptr[i] = obj.vertexes[i];
		indices_ptr[i] = i;
	}
	obj.VB->Unlock();
	obj.IB->Unlock();
}