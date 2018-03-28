#ifndef MTL_LOADER__
#define MTL_LOADER__

#include <string>
#include <map>

namespace objl{
	
	struct mtl{
		float ambient_color[3];		//Ka
		float diffuse_color[3];		//Kd
		float specular_color[3];	//Ks
		float specular_exponent;	//Ns
		float transparent;			//	d 0.9     some implementations use 'd'
									//	Tr 0.1    others use 'Tr' (inverted: Tr = 1 - d)
		int illumination_models;	//illum 
			/*	0. Color on and Ambient off
			1. Color on and Ambient on
			2. Highlight on
			3. Reflection on and Ray trace on
			4. Transparency: Glass on, Reflection: Ray trace on
			5. Reflection: Fresnel on and Ray trace on
			6. Transparency: Refraction on, Reflection: Fresnel off and Ray trace on
			7. Transparency: Refraction on, Reflection: Fresnel on and Ray trace on
			8. Reflection on and Ray trace off
			9. Transparency: Glass on, Reflection: Ray trace off
			10. Casts shadows onto invisible surfaces*/
		std::string ambient_texture;		//map_kA
		std::string diffuse_texture;		//map_Kd
		std::string specular_color_texture;	//map_kS
		std::string specular_highlight;		//map_Ns
		std::string alpha_texture;			//map_d
		std::string bump;					//bump
											//disp
											//decal
											//map_opacity
		std::string reflection;				//refl
	};
	
	std::map<std::string, mtl> 
	mtl_loader (const std::string& path, const std::string& filename);
}

#endif