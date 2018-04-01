#ifndef OBJ_LOADER__
#define OBJ_LOADER__

#include "mtl_loader.hpp"
#include "mtl_indices.hpp"

#include <string>
#include <fstream>
#include <cstddef>

#include <deque>
#include <list>
#include <map>

namespace objl{
	
	struct vertex;
	struct normal;
	struct texture_uv;
	struct triangle;
	struct group;
	
	struct object{
		typedef std::deque<vertex> 			vertex_container_type;
		typedef std::deque<normal> 			normal_container_type;
		typedef std::deque<texture_uv> 		texture_uv_container_type;
		typedef std::deque<triangle> 		triangle_container_type;
		typedef std::list<group> 			group_container_type;		
		typedef std::map<std::string, mtl> 	material_container_type;
		typedef std::deque<mtl_indices> 	mtl_indices_container_type;
		
		vertex_container_type 		vertexes;
		normal_container_type 		normals;
		texture_uv_container_type 	textures_uv;
		triangle_container_type		triangles;
		group_container_type		groups;
		
		material_container_type 	materials;
		mtl_indices_container_type	mtl_indices;
	};
	
	object obj_loader(const std::string& path, const std::string& filename);
	
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
		typedef std::size_t vertex_index_type;
		typedef std::size_t texture_uv_index_type;
		typedef std::size_t normal_index_type;
		
		vertex_index_type 		vertex_first, 	vertex_second, 	vertex_third;
		texture_uv_index_type 	uv_first, 		uv_second, 		uv_third;
		normal_index_type 		normal_first, 	normal_second, 	normal_third;
	};
	
	struct group{
		std::string name;
		std::size_t start_index;
	};
}

#endif