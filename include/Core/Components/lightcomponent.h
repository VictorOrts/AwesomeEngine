#ifndef _H_LIGHT_COMPONENT_
#define _H_LIGHT_COMPONENT_

#include "Core/types.h"
#include "Core/component.h"
#include "Core/Light/light.h"

namespace Awesome {

	/**
	 * @brief      LightComponent class ,where the gameobjects can be a light instead of Mesh
	 */
	class LightComponent : public Component
	{
	public:
		LightComponent();
		~LightComponent();

		void update();
		void render(mat4 transform,uint32 program);
		void func() {
			printf("DIRLIGHTCOMPONENT::FUNC");
		}
		std::shared_ptr<Light> getLight();
		void addLight(std::shared_ptr<Light>);
		string name() {
			return name_;
		}
	private:
		string name_;
		std::shared_ptr<Light> light_;
	};
}

#endif // !_H_LIGHT_COMPONENT_
