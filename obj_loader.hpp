#ifndef OBJ_LOADER__
#define OBJ_LOADER__

#include <string>
#include <fstream>

#include <deque>

#include <iostream>

namespace objl{
	
	struct vertex{
		float x, y, z;
	};
	
	struct normal{
		float xn, yn, zn;
	};
	
	struct texture{
		float u, v;
	};
	
	struct triangle{
		unsigned int ix1, ix2, ix3;
	};
	
	void obj_loader(const std::string& filename){
		
		std::fstream obj_file(filename, std::ios::in);	
		std::string cmd;
		
		std::deque<vertex> vertexes(4096);
		std::deque<normal> normals(4096);
		
		while(!obj_file.eof()){
			if(cmd == "#"){
				std::getline(obj_file, cmd);
			} else if (cmd == "v"){
				vertex new_vertex{};
				std::string x, y, z, w;
				obj_file >> x >> y >> z >> w;
				
				new_vertex.x = stof(x);
				new_vertex.y = stof(y);
				new_vertex.z = stof(z);
				
				if(std::isalpha(*(w.c_str())))
					cmd = w;
				else
					obj_file >> cmd;
				
				std::cerr << new_vertex.x << ' ' 
					<< new_vertex.y << ' ' << new_vertex.z << '\n';
				
				vertexes.push_back(new_vertex);
			} else if(cmd == "vn"){
				normal new_normal{};
				std::string xn, yn, zn;
				obj_file >> xn >> yn >> zn;
				
				new_normal.xn = stof(xn);
				new_normal.yn = stof(yn);
				new_normal.zn = stof(zn);
				
				normals.push_back(new_normal);
				
				obj_file >> cmd;
			} else
				obj_file >> cmd;
		}
	}
	
}

#endif