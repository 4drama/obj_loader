#ifndef OBJ_LOADER__
#define OBJ_LOADER__

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
	
}

#endif