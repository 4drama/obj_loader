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
	
	material create_material(objl::mtl &material_s, std::string path){		
		material res{};
		if(material_s.diffuse_texture.size() != 0){
			res.diffuse_texture_full_path = path.append(material_s.diffuse_texture);
		}
		
		res.material.Diffuse = D3DCOLORVALUE{
			material_s.diffuse_color[0],
			material_s.diffuse_color[1],
			material_s.diffuse_color[2],
			1.0f};
		res.material.Ambient = D3DCOLORVALUE{
			material_s.ambient_color[0],
			material_s.ambient_color[1],
			material_s.ambient_color[2],
			1.0f
			};
		res.material.Specular = D3DCOLORVALUE{
			material_s.specular_color[0],
			material_s.specular_color[1],
			material_s.specular_color[2],
			1.0f
			};
			
		res.material.Emissive = (D3DXCOLOR)D3DCOLOR_XRGB(0, 0, 0);
		res.material.Power = material_s.specular_exponent; 	//????
		return res;
	}
}

D3DXMATRIX get_transform_matrix(dx_obj &obj){
	return obj.scaling * obj.rotation_x
			* obj.rotation_y * obj.rotation_z * obj.translation;
}

dx_obj load_file(const std::string& path, const std::string& filename){	
	objl::object obj = objl::obj_loader(path, filename);
	dx_obj result;

	for(auto &current_index : obj.mtl_indices){
		result.materials_indices.push_back( mtl_indices{
			current_index.name, 
			(current_index.begin * 3) - 3, 
			(current_index.end * 3) - 1});
	}
	
	for(auto &current_material : obj.materials){	
		result.materials[current_material.first]
			= create_material(current_material.second, path);
	}
	
	for(auto &current_triangle : obj.triangles){
		
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_first - 1],
				obj.normals[current_triangle.normal_first - 1],
				obj.textures_uv[current_triangle.uv_first - 1]));
				
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_second - 1],
				obj.normals[current_triangle.normal_second - 1],
				obj.textures_uv[current_triangle.uv_second - 1]));
				
		result.vertexes.push_back(create_vertex(
				obj.vertexes[current_triangle.vertex_third - 1],
				obj.normals[current_triangle.normal_third - 1],
				obj.textures_uv[current_triangle.uv_third - 1]));		
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
	
	obj.VB->Lock(0, 0, (void**)&vertices_ptr, 0);
	
	for(int i = 0; i != obj.vertexes.size(); ++i){
		vertices_ptr[i] = obj.vertexes[i];
	}
	obj.VB->Unlock();
	obj.triangles_size = obj.vertexes.size() / 3;
	
	
	for(auto &curr_mtl : obj.materials){
		if(curr_mtl.second.diffuse_texture_full_path.size() != 0){
			D3DXCreateTextureFromFile(
				device,
				curr_mtl.second.diffuse_texture_full_path.c_str(),
				&curr_mtl.second.diffuse_texture);
		}
	}
}