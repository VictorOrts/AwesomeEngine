/*
*	Mesh class
*	Basic class for Cube Plane and Obj.
*	by Victor Orts
*/
#ifndef _H_MESH_
#define _H_MESH_

#include <vector>
#include "types.h"
namespace Awesome {
	class Mesh  {
	public:
		Mesh();
		~Mesh();
		uint32 get_evo();
		uint32 get_vao();
		uint32 get_numindices();
		string get_name();
		void Create();
		virtual void Start() = 0;
	protected:
		void trueCreate();
		uint32 vao_;
		uint32 evo_;
		uint32 numindices;
		std::vector<float> vertex;
		std::vector<float> normals;
		std::vector<float> uv;
		std::vector<uint32> index;
		string name_;
	};
}

#endif // !_H_MESH_
