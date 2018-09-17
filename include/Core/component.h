#ifndef _H_COMPONENT_
#define _H_COMPONENT_

#include "types.h"

namespace Awesome {
	class GameObject;
	class Entity;
	enum ComponentType {
		kRenderComponent = 0,
		kScriptComponent,
		kLightComponent,
		KSineTransform
	};
	class Component {
	public:
		


		Component();
		~Component();
		ComponentType get_type_component() const ;
		virtual void update() = 0;
		virtual void render(mat4 transform,uint32 program) = 0;

		/**
		 * @brief      Sets the entity whe add a component to a entity
		 *
		 * @param[in]  rgo   The rgo
		 */
		void set_game_object(std::shared_ptr<Entity> rgo);
		std::shared_ptr<Entity> game_object();
		vec3 position();

	protected:
		std::shared_ptr<Entity> go_;
		ComponentType type_component_;
		bool enabled_;
	};
}

#endif // !_H_COMPONENT_
