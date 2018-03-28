#include "mtl_loader.hpp"

#include <fstream>

namespace{
	void material_parser(	std::map<std::string, objl::mtl> 		&materials,
							std::fstream 							&mtl_file,
							std::string								&cmd);
}

std::map<std::string, objl::mtl> 
objl::mtl_loader (const std::string& path, const std::string& filename){
	
	std::string full_path = path;
	full_path.append(filename);
	
	std::fstream mtl_file(full_path, std::ios::in);	
	std::string cmd;
	
	std::map<std::string, objl::mtl> load_materials;
	
	while(!mtl_file.eof()){
		if(cmd == "#"){
			std::getline(mtl_file, cmd);
		} else if (cmd == "newmtl"){
			material_parser(load_materials, mtl_file, cmd);
		} else
			mtl_file >> cmd;		
	}
	
	return load_materials;
}

namespace{
	void material_parser(	std::map<std::string, objl::mtl> 		&materials,
							std::fstream 							&mtl_file,
							std::string								&cmd){
		std::string name;
		objl::mtl material{};
		
		mtl_file >> name;
		
		while((cmd != "newmtl") || (!mtl_file.eof())){
			if(cmd == "#"){
				std::getline(mtl_file, cmd);
			} else if (cmd == "Ka"){
			} else if (cmd == "Kd"){
			} else if (cmd == "Ks"){
			} else if (cmd == "Ns"){
			} else if (cmd == "d"){
			} else if (cmd == "Tr"){
			} else if (cmd == "illum"){
			} else if (cmd == "map_kA"){
			} else if (cmd == "map_Kd"){
			} else if (cmd == "map_kS"){
			} else if (cmd == "map_Ns"){
			} else if (cmd == "map_d"){
			} else if (cmd == "bump" || cmd == "map_bump"){
			} else if (cmd == "disp"){
			} else if (cmd == "decal"){
			} else if (cmd == "map_opacity"){
			} else if (cmd == "refl"){
			} else
				mtl_file >> cmd;	
		}
	}
}