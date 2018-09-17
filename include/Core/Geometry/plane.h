#ifndef _H_PLANE_
#define _H_PLANE_

#include "Core/mesh.h"

namespace Awesome {
	class Plane :public Mesh
	{
	public:
		enum TypePlane
		{
			kPlane_RTT,
			kPlane_Plane
		};
		Plane(TypePlane a=kPlane_Plane);
		~Plane();
		void Start();
	private:
	};
}

#endif // !_PLANE_H_
