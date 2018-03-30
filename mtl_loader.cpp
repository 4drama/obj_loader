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
									
	void illumination_models_parser(	objl::mtl &material, 
										std::fstream &mtl_file, 
										std::string &cmd);
	
	void texture_parser (		std::string &texture, 
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
				specular_exponent_parser(material, mtl_file, cmd);
			} else if (cmd == "d"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "Tr"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "illum"){
				illumination_models_parser(material, mtl_file, cmd);
			} else if (cmd == "map_kA"){
				texture_parser(material.ambient_texture, mtl_file, cmd);
			} else if (cmd == "map_Kd"){
				texture_parser(material.diffuse_texture, mtl_file, cmd);
			} else if (cmd == "map_kS"){
				texture_parser(material.specular_color_texture, mtl_file, cmd);
			} else if (cmd == "map_Ns"){
				texture_parser(material.specular_highlight, mtl_file, cmd);
			} else if (cmd == "map_d"){
				texture_parser(material.alpha_texture, mtl_file, cmd);
			} else if (cmd == "bump" || cmd == "map_bump"){
				texture_parser(material.bump, mtl_file, cmd);
			} else if (cmd == "disp"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "decal"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "map_opacity"){
				mtl_file >> cmd;	//TODO
			} else if (cmd == "refl"){
				texture_parser(material.reflection, mtl_file, cmd);
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
	
	std::stringstream get_stream_line(std::fstream &file){
		std::string line;
		std::stringstream line_stream;
		std::getline(file, line);
		line_stream << line;
		return line_stream;
	}
	
	void three_arguments_parcer(	float arg[], 
									std::fstream &mtl_file, 
									std::string &cmd){
		
		std::stringstream line_stream = get_stream_line(mtl_file);

		try {
			for(int i = 0; i != 3; i++){
				if(!line_stream.eof()){
					line_stream >> arg[i];
				}else
					throw std::range_error("insufficient data");
			}
		} catch(const std::range_error& e){
			for(int i = 0; i != 3; i++)
				arg[i] = 0;
		} catch(const std::exception& e){
			throw e;
		}

		mtl_file >> cmd;	
	}
	
	void ambient_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		
		three_arguments_parcer(material.ambient_color, mtl_file, cmd);
	}
	
	void diffuse_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		three_arguments_parcer(material.diffuse_color, mtl_file, cmd);
	}
	
	void specular_color_parser(	objl::mtl &material, 
								std::fstream &mtl_file, 
								std::string &cmd){
		three_arguments_parcer(material.specular_color, mtl_file, cmd);
	}
	
	template<class T>
	void arg_save(std::string &line, T &arg){
		std::stringstream line_stream;
		line_stream << line.substr(1);
		line_stream >> arg;
	}
	
	void arg_save(std::string &line, std::string &arg){
		arg =  line.substr(1);
	}
	
	template<class T>
	int single_parser(	T &arg, 
						std::fstream &file, 
						std::string &cmd){
		
		std::string line;
		std::getline(file, line);
		file >> cmd;
		
		if((line[0] != ' ') || (line.size() <= 1)){
			return -1;
		} else {		
			arg_save(line, arg);
			return 0;
		}
	}
	
	void illumination_models_parser(	objl::mtl &material, 
										std::fstream &mtl_file, 
										std::string &cmd){		
		if(single_parser<int>(material.illumination_models, mtl_file, cmd) != 0)
			material.illumination_models = 0;
	}
	
	void specular_exponent_parser(	objl::mtl &material, 
									std::fstream &mtl_file, 
									std::string &cmd){
		if(single_parser<float>(material.specular_exponent, mtl_file, cmd) != 0)
			material.specular_exponent = 0;
	}
	
	void texture_parser (		std::string &texture, 
								std::fstream &mtl_file, 
								std::string &cmd){
		single_parser<std::string>(texture, mtl_file, cmd);
	}
}