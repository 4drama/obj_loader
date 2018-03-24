#include "dx_obj.hpp"

#include "obj_loader.hpp"

#include <stdexcept>
#include <iostream>

const DWORD vertex::FVF = D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1;

dx_obj load_file(const std::string& filename){	
	objl::object obj = objl::obj_loader(filename);
	
/*	if( (	obj.vertexes.size() != obj.normals.size()) ||
		(	obj.normals.size() != obj.textures_uv.size()) ){
		std::cerr << "obj.vertexes.size " << obj.vertexes.size() << std::endl;
		std::cerr << "obj.normals.size " << obj.normals.size() << std::endl;
		std::cerr << "obj.textures_uv.size " << obj.textures_uv.size() << std::endl;
		throw std::logic_error("Object not full.");
	}*/
	
	dx_obj result;
/*	result.vertexes.reserve(obj.vertexes.size());
	result.vertexes.resize(obj.vertexes.size());
	
	for(int i = 0; i != obj.vertexes.size(); ++i){
		vertex v{obj.vertexes[i].x, obj.vertexes[i].y, obj.vertexes[i].z,
			obj.normals[i].xn, obj.normals[i].yn, obj.normals[i].zn,
			obj.textures_uv[i].u, obj.textures_uv[i].v};
		
		result.vertexes[i] = v;
	}*/
	
	return result;
}