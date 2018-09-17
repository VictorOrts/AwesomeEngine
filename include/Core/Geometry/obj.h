#ifndef _H_OBJ_
#define _H_OBJ_

#include "Core/mesh.h"
#include "deps/glm/gtx/hash.hpp"
namespace Awesome {
	class Obj : public Mesh {
	public:
		Obj(string name);
		~Obj();
		void TrueLoad();
		void Start();
	};
}

#endif // !_H_OBJ_
