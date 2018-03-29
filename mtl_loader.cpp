#include "mtl_loader.hpp"

#include <fstream>
#include <istream>
#include <sstream>

#include <stdexcept>

#include <iostream>

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
	void debug_print(std::string &name, objl::mtl &material);
	
	void ambient_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd);
	
	void diffuse_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd);
	
	void specular_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd);
	
	void specular_exponent_parser(	objl::mtl &material, 
									std::fstream &mtl_file, 
									std::string &cmd);
}

namespace{
	void material_parser(	std::map<std::string, objl::mtl> 		&materials,
							std::fstream 							&mtl_file,
							std::string								&cmd){
		std::string name;
		objl::mtl material{};
		
		mtl_file >> name >> cmd;
		
		while( !(	(cmd == "newmtl") || (mtl_file.eof())	)	){
			if(cmd == "#"){
				std::getline(mtl_file, cmd);
			} else if (cmd == "Ka"){
				ambient_color_parser(material, mtl_file, cmd);
			} else if (cmd == "Kd"){
				diffuse_color_parser(material, mtl_file, cmd);
			} else if (cmd == "Ks"){
				specular_color_parser(material, mtl_file, cmd);
			} else if (cmd == "Ns"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "d"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "Tr"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "illum"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_kA"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_Kd"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_kS"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_Ns"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_d"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "bump" || cmd == "map_bump"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "disp"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "decal"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_opacity"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "refl"){
				mtl_file >> cmd;	//TODO
			} else
				mtl_file >> cmd;	
		}
		
		debug_print(name, material);
	}
}

namespace{
	void debug_print(std::string &name, objl::mtl &material){
		std::cerr << name << std::endl;
		
		std::cerr 	<< "ambient_color " << material.ambient_color[0] << ' '
					<< material.ambient_color[1] << ' ' << material.ambient_color[2]
					<< std::endl;
		
		std::cerr 	<< "diffuse_color " << material.diffuse_color[0] << ' '
					<< material.diffuse_color[1] << ' ' << material.diffuse_color[2]
					<< std::endl;
		
		std::cerr 	<< "specular_color " << material.specular_color[0] << ' '
					<< material.specular_color[1] << ' ' << material.specular_color[2]
					<< std::endl;
		
		std::cerr 	<< "specular_exponent " << material.specular_exponent << std::endl;
		std::cerr 	<< "illumination_models " << material.illumination_models 
					<< std::endl;
		
		std::cerr 	<< "ambient_texture " << material.ambient_texture << std::endl;
		std::cerr 	<< "diffuse_texture " << material.diffuse_texture << std::endl;
		std::cerr 	<< "specular_color_texture " << material.specular_color_texture 
					<< std::endl;
		
		std::cerr 	<< "specular_highlight " << material.specular_highlight << std::endl;
		std::cerr 	<< "alpha_texture " << material.alpha_texture << std::endl;
		std::cerr 	<< "bump " << material.bump << std::endl;
		std::cerr 	<< "reflection " << material.reflection << std::endl;
		
		std::cerr << std::endl;
	}
	
	void ambient_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		
	std::string line;
	std::stringstream line_stream;
	std::getline(mtl_file, line);
	line_stream << line;
	
	try {
		for(int i = 0; i != 3; i++){
			if(!line_stream.eof()){
				line_stream >> material.ambient_color[i];
			}else
				throw std::range_error("insufficient data");
		}
	} catch(const std::range_error& e){
		for(int i = 0; i != 3; i++)
			material.ambient_color[i] = 0;
	} catch(const std::exception& e){
		throw e;
	}
	
	mtl_file >> cmd;
	
	}
	
	void diffuse_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		mtl_file 	>> material.diffuse_color[0]
					>> material.diffuse_color[1]
					>> material.diffuse_color[2]
					>> cmd;
	}
	
	void specular_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		mtl_file 	>> material.specular_color[0]
					>> material.specular_color[1]
					>> material.specular_color[2]
					>> cmd;
	}
	
	void specular_exponent_parser(	objl::mtl &material, 
									std::fstream &mtl_file, 
									std::string &cmd){
		mtl_file	>> material.specular_exponent >> cmd;
	}
}