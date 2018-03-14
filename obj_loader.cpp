#include "obj_loader.hpp"

#include <iostream>

namespace{
	void vertex_parser(	objl::object::vertex_container_type 	&vertexes,
							std::fstream 							&obj_file,
							std::string								&next_cmd);
	
	void normal_parser(	objl::object::normal_container_type 	&normals,
							std::fstream 							&obj_file,
							std::string								&next_cmd);
	
	void texture_uv_parser(	objl::object::texture_uv_container_type 	&textures_uv,
							std::fstream 								&obj_file,
							std::string									&next_cmd);
}

objl::object objl::obj_loader(const std::string& filename){
	
	std::fstream obj_file(filename, std::ios::in);	
	std::string cmd;
	
	objl::object load_objct;
	
	while(!obj_file.eof()){
		if(cmd == "#"){
			std::getline(obj_file, cmd);
		} else if (cmd == "v"){
			vertex_parser(load_objct.vertexes, obj_file, cmd);
		} else if(cmd == "vn"){
			normal_parser(load_objct.normals, obj_file, cmd);
		} else if(cmd == "vt"){
			texture_uv_parser(load_objct.textures_uv, obj_file, cmd);
		} else
			obj_file >> cmd;
		
	}
	
	std::cerr << load_objct.vertexes.size();
	return load_objct;
}

namespace{
	void vertex_parser(	objl::object::vertex_container_type 	&vertexes,
						std::fstream 							&obj_file,
						std::string								&next_cmd){
		objl::vertex new_vertex{};
		std::string x, y, z, w;
		obj_file >> x >> y >> z >> w;
		
		new_vertex.x = stof(x);
		new_vertex.y = stof(y);
		new_vertex.z = stof(z);
		
		if(std::isalpha(*(w.c_str())))
			next_cmd = w;
		else
			obj_file >> next_cmd;
		
		std::cerr << new_vertex.x << ' ' 
			<< new_vertex.y << ' ' << new_vertex.z << '\n';
		
		vertexes.push_back(new_vertex);
	}
	
	void normal_parser(	objl::object::normal_container_type 	&normals,
							std::fstream 							&obj_file,
							std::string								&next_cmd){
		objl::normal new_normal{};
		std::string xn, yn, zn;
		obj_file >> xn >> yn >> zn;
		
		new_normal.xn = stof(xn);
		new_normal.yn = stof(yn);
		new_normal.zn = stof(zn);
		
		normals.push_back(new_normal);
		
		obj_file >> next_cmd;
	}
	
	void texture_uv_parser(	objl::object::texture_uv_container_type 	&textures_uv,
							std::fstream 								&obj_file,
							std::string									&next_cmd){
		objl::texture_uv new_uv{};
		std::string u, v;
		
		obj_file >> u >> v;
		
		new_uv.u = stof(u);
		new_uv.v = stof(v);
		
		textures_uv.push_back(new_uv);
		
		obj_file >> next_cmd;
	}
}