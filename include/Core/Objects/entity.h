#ifndef _H_ENTITY_
#define _H_ENTITY_

#include <vector>
#include "Core/types.h"
#include "Core/Objects/utils.h"
#include "Core/component.h"
namespace Awesome {
	struct cerealEntity;
	class Entity  :public std::enable_shared_from_this<Entity> 
	{
	public:
		Entity();
		~Entity();
		template <class Composite>
		void addComponent() {
			std::shared_ptr<Component> a = std::make_shared<Composite>();
			a->set_game_object(shared_from_this());
			list_.push_back(a);
		}
		template <class Composite>
		std::shared_ptr<Composite> getComponent(ComponentType s) {
			for (uint16 i = 0; i < list_.size(); i++) {
				if (s == list_[i]->get_type_component()) {
					//Utils::Log("SUCCES::GET_COMPONENT_FIND", kLogColors::kColor_Green, kLogOutput::kWindow_CMD);
					std::shared_ptr<Composite>  a = std::dynamic_pointer_cast<Composite>(list_[i]);
					return a;
				}

			}
			Utils::Log("ERROr::GET_COMPONENT_NOT_FIND", kLogColors::kColor_Red, kLogOutput::kWindow_CMD);
			return nullptr;
		}
		void update();
		void deleteComponent(ComponentType s);
		bool hasComponent(ComponentType s);
		template <class NewEntity>
		std::shared_ptr<NewEntity> CreateEntity() {
			return std::make_shared<NewEntity>();
		}
		vec3 getPosition();
		void position(vec3 pos);
		void position(float x, float y, float z);

		vec3 getScale();
		void scale(vec3 scale_);
		void scale(float x, float y, float z);

		vec3 getRotation();
		void rotation(vec3 rot);
		void rotation(float x, float y, float z);

		void move(float x, float y, float z);
		void rotate(float x, float y, float z);

		mat4 transform();
		void setWorld(mat4 n_world);
		mat4 world();
		//NODES

		void processnode(std::shared_ptr<Entity>);
		void visitornode(std::shared_ptr<Entity>);

		void addChild(std::shared_ptr<Entity> child_);
		void setParent(std::shared_ptr<Entity> parent_);

		//Get child and parent
		std::shared_ptr<Entity> getParent();
		std::vector < std::shared_ptr<Entity>> getChildren();
		//Remove
		void removeParent();
		void destroyChild(std::shared_ptr<Entity> node_);
		void DestroyChildren();

		//Name Entity
		void set_name(const string name_);
		string name() const ;
		
		void CerealLoad(cerealEntity a);
	private:
		//Nodes
		std::shared_ptr<Entity> parent_;
		std::vector<std::shared_ptr<Entity>> children_;
		string name_;
		///////
		mat4 updateModel(vec3 pos, vec3 scale, vec3 rota);
		mat4 world_;
		mat4 transform_;
		vec3 position_;
		vec3 scale_;
		vec3 rotation_;
		bool model_dirty_;
		std::vector<std::shared_ptr<Component>> list_;
	};

	
}


#endif // !_H_ENTITY_
