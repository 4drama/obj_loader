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

	void triangle_parser(	objl::object::triangle_container_type	 	&triangles,
							std::fstream 								&obj_file,
							std::string									&next_cmd);
	
	void group_parser(	objl::object::group_container_type	 		&groups,
						objl::object::vertex_container_type			&vertexes,
						std::fstream 								&obj_file,
						std::string									&next_cmd);
	
	void usemtl_parser(	objl::object::mtl_indices_container_type	&indices,
						objl::object::triangle_container_type		&triangles,
						std::fstream 								&obj_file,
						std::string									&next_cmd);
	
	void usemtl_last_check(	objl::object::mtl_indices_container_type	&indices,
							objl::object::triangle_container_type		&triangles);
	
	void mtl_debug_print(	objl::object::mtl_indices_container_type	&indices);
}

objl::object objl::obj_loader(const std::string& path, const std::string& filename){
	
	std::string full_path = path;
	full_path.append(filename);
	
	std::fstream obj_file(full_path, std::ios::in);	
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
		} else if(cmd == "f"){
			triangle_parser(load_objct.triangles, obj_file, cmd);
		} else if(cmd == "g"){
			group_parser(load_objct.groups, load_objct.vertexes, obj_file, cmd);
		} else if(cmd == "usemtl"){
			usemtl_parser(load_objct.mtl_indices, load_objct.triangles, obj_file, cmd);
		} else if(cmd == "mtllib"){
			std::string mtl_file_name;
			obj_file >> mtl_file_name;
			load_objct.materials = objl::mtl_loader(path, mtl_file_name);
			obj_file >> cmd;
		} else if(cmd == "o"){
			obj_file >> cmd;	//TODO
		} else
			obj_file >> cmd;		
	}
	
	usemtl_last_check(load_objct.mtl_indices, load_objct.triangles);
//	mtl_debug_print(load_objct.mtl_indices);
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
	
	void parse_arg(	const std::string &arg, 
					objl::triangle::vertex_index_type &i,
					objl::triangle::texture_uv_index_type &uv,
					objl::triangle::normal_index_type &n){
		
		std::string::size_type first_slash = arg.find('/', 1);
		std::string::size_type second_slash = arg.find('/', first_slash + 1);
		
		if (first_slash == std::string::npos)
			first_slash = arg.length();
		
		if (second_slash == std::string::npos)
			second_slash = arg.length();
		
		i = stoi(arg.substr(0, first_slash));
		
		if((second_slash - first_slash) <= 1)
			uv = 0;
		else
			uv = stoi(arg.substr(first_slash + 1, second_slash));
		
		if(second_slash != arg.length())
			n = stoi(arg.substr(second_slash + 1, arg.length()));
		 else 
			n = 0;
	}
	
	void triangle_parser(	objl::object::triangle_container_type	 	&triangles,
							std::fstream 								&obj_file,
							std::string									&next_cmd){
		objl::triangle new_triangle{};
		std::string first_arg, second_arg, third_arg;
		
		obj_file >> first_arg >> second_arg >> third_arg;
		
		parse_arg(first_arg, new_triangle.vertex_first,
				new_triangle.uv_first, new_triangle.normal_first);
		
		parse_arg(second_arg, new_triangle.vertex_second,
				new_triangle.uv_second, new_triangle.normal_second);
		
		parse_arg(third_arg, new_triangle.vertex_third,
				new_triangle.uv_third, new_triangle.normal_third);
		
		triangles.push_back(new_triangle);
		
		obj_file >> next_cmd;
	}
	
	void group_parser(	objl::object::group_container_type	 		&groups,
						objl::object::vertex_container_type			&vertexes,
						std::fstream 								&obj_file,
						std::string									&next_cmd){
		objl::group new_group{};
		std::string name;
		
		obj_file >> name;
		
		new_group.name = name;
		new_group.start_index = vertexes.size();
		
		groups.push_back(new_group);
		
		obj_file >> next_cmd;
	}
	
	void usemtl_parser(	objl::object::mtl_indices_container_type	&indices,
						objl::object::triangle_container_type		&triangles,
						std::fstream 								&obj_file,
						std::string									&next_cmd){
		mtl_indices new_index{};
		
		obj_file >> new_index.name;
		new_index.begin = triangles.size() + 1;
		
		usemtl_last_check(indices, triangles);
		
		indices.push_back(new_index);
		
		obj_file >> next_cmd;
	}
	
	void usemtl_last_check(	objl::object::mtl_indices_container_type	&indices,
							objl::object::triangle_container_type		&triangles){
		if(indices.size() != 0)
			indices.back().end = triangles.size();
	}
	
	void mtl_debug_print(objl::object::mtl_indices_container_type	&indices){
		for(auto &curr : indices){
			std::cerr 	<< curr.name << ' ' << curr.begin << '-' << curr.end
						<< std::endl;
		}
	}
}