#ifndef _H_RENDER_COMPONENT_
#define _H_RENDER_COMPONENT_
#include "Core/component.h"
#include "Core/mesh.h"
#include "Core/material.h"

namespace Awesome {
	class RenderComponent :public Component {
	public:
		RenderComponent();
		~RenderComponent();
		template <class TMesh>
		void addMesh();
		template <class TMesh>
		void addMesh(std::shared_ptr<TMesh> mesh) {
			mesh_ = mesh;
		}
		template <class TMesh>
		//Valid method for OBJ
		void addMesh(string);

		void addMaterial();
		std::shared_ptr<Mesh> mesh_component() const ;
		std::shared_ptr<Material> material_component() const ;
		void update();
		void render(mat4 transform, uint32 program);
		void prueba() {
			printf("\nEsto funciona guys");
		}
	private:
		std::shared_ptr<Mesh> mesh_;
		std::shared_ptr<Material> material_;
	};
}

#endif // !_H_RENDER_COMPONENT_
