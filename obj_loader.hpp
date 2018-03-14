#ifndef OBJ_LOADER__
#define OBJ_LOADER__

#include <string>
#include <fstream>

#include <deque>

namespace objl{
	
	struct vertex{
		float x, y, z;
	};
	
	struct normal{
		float xn, yn, zn;
	};
	
	struct texture_uv{
		float u, v;
	};
	
	struct triangle{
		unsigned int ix1, ix2, ix3;
	};
	
	struct object{
		typedef std::deque<vertex> vertex_container_type;
		typedef std::deque<normal> normal_container_type;
		typedef std::deque<texture_uv> texture_uv_container_type;
		
		vertex_container_type vertexes;
		normal_container_type normals;
		texture_uv_container_type textures_uv;
	};
	
	object obj_loader(const std::string& filename);
	
}

#endif