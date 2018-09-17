#include "Core/Objects/entity.h"
#include "Core/engine.h"
#include "Core/Task/gameobjectstask.h"
#include "deps/glm/gtx/transform.hpp"
#include "Core/Objects/mycerealsaves.h"
using namespace Awesome;

Entity::Entity()
{
	transform_ = mat4(1.0f);
	world_ = mat4(1.0f);
	
	parent_ = nullptr;
	position_ = vec3(0);
	scale_ = vec3(1);
	rotation_ = vec3(0);
	model_dirty_ = true;
	name_ = "no_name";
	//typeGO_ = TypeGameObject::kGameObject_NoRenderable;
}

Entity::~Entity()
{
	list_.clear();
}


void Entity::update() {
	if (model_dirty_) {
		std::shared_ptr<UpdateGoTask> goTask = std::make_shared<UpdateGoTask>(shared_from_this());
		Engine::engine_instance()->get_task()->AddTask(goTask);
		model_dirty_ = false;
	}

	for (uint16 i = 0; i < list_.size(); i++) {
		list_[i]->update();
	}
}


void Entity::deleteComponent(ComponentType s) {
	for (uint16 i = 0; i < list_.size(); i++) {
		if (s == list_[i]->get_type_component()) {
			list_.erase(list_.begin() + i);
		}
	}
}


bool Entity::hasComponent(ComponentType s) {
	for (uint16 i = 0; i < list_.size(); i++) {
		if (s == list_[i]->get_type_component()) {
			//printf("\nTiene el componente");
			return true;
		}

	}
	return false;
}

//////  TRANSFORMS  ////////

vec3 Entity::getPosition() {
	return position_;
}
vec3 Entity::getScale() {
	return scale_;
}

vec3 Entity::getRotation() {
	return rotation_;
}

void Entity::position(vec3 pos) {
	if (position_ != pos) {
		position_ = pos;
		transform_ = updateModel(position_, rotation_, scale_);
	}
}
void Entity::position(float x, float y, float z) {
	vec3 tmpPosition_ = vec3(x, y, z);
	if (position_ != tmpPosition_) {
		position_ = tmpPosition_;
		transform_ = updateModel(position_, rotation_, scale_);
	}
}
void Entity::scale(vec3 scale) {
	if (scale_ != scale) {
		scale_ = scale;
		transform_ = updateModel(position_, rotation_, scale_);
	}
}
void Entity::scale(float x, float y, float z) {
	vec3 tmpScale_ = vec3(x, y, z);
	if (scale_ != tmpScale_) {
		scale_ = tmpScale_;
		transform_ = updateModel(position_, rotation_, scale_);
	}
}

void Entity::rotation(vec3 rot) {
	if (rotation_ != rot) {
		rotation_ = rot;

		float max = glm::radians(360.f);
		while (rotation_.x >= max) rotation_.x -= max;
		while (rotation_.y >= max) rotation_.y -= max;
		while (rotation_.z >= max) rotation_.z -= max;

		transform_ = updateModel(position_, rotation_, scale_);
	}
}

void Entity::rotation(float x, float y, float z) {
	vec3 r_rot = vec3(x, y, z);
	if (rotation_ != r_rot) {
		rotation_ = r_rot;

		float max = glm::radians(360.f);
		while (rotation_.x >= max) rotation_.x -= max;
		while (rotation_.y >= max) rotation_.y -= max;
		while (rotation_.z >= max) rotation_.z -= max;
		transform_ = updateModel(position_, rotation_, scale_);
	}
}

void Entity::move(float x, float y, float z) {
	vec3 current_position = position_ + vec3(x, y, z)*(2 * Engine::engine_instance()->get_time()->time_elapsed());
	position(current_position);
}
void Entity::rotate(float x, float y, float z) {
	vec3 current_rotation = rotation_ + vec3(glm::radians(x), glm::radians(y), glm::radians(z));
	rotation(current_rotation);
}


mat4 Entity::updateModel(vec3 position, vec3 rotation, vec3 scale) {
	model_dirty_ = true;
	mat4 rotationMatrix(1.f);
	rotationMatrix = glm::rotate(rotationMatrix, rotation.x, vec3(1.0f, 0.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.y, vec3(0.0f, 1.0f, 0.0f));
	rotationMatrix = glm::rotate(rotationMatrix, rotation.z, vec3(0.0f, 0.0f, 1.0f));
	return translate(position) * rotationMatrix * glm::scale(scale);
}


mat4 Entity::transform() {
	return transform_;
}

void Entity::setWorld(mat4 rmat) {
	world_ = rmat;
}

mat4 Entity::world() {
	return world_;
}


//NODES

void Entity::addChild(std::shared_ptr<Entity> child_) {
	std::shared_ptr<Entity> tmpChild_ = std::make_shared<Entity>();
	child_->setParent(shared_from_this());
	children_.push_back(child_);
}
void Entity::destroyChild(std::shared_ptr<Entity> node_) {
	for (uint32 i = 0; i < children_.size(); i++) {
		if (children_[i] == node_) {
			printf("\nSon iguales");
			children_[i]->removeParent();
			children_[i]->scale(vec3(1));
			children_.erase(children_.begin() + i);
		}
		else {
			printf("ERROR::NODE::NODE_NOT_EXIST");
		}
	}
}

void Entity::DestroyChildren() {
	for (uint32 i = 0; i < children_.size(); i++) {
		children_[i]->removeParent();
	}
	children_.clear();
}

void Entity::setParent(std::shared_ptr<Entity> rparent_) {
	parent_ = rparent_;
}

std::shared_ptr<Entity> Entity::getParent() {
	if (parent_ == nullptr) {
		return nullptr;
	}
	else
	{
		return parent_;
	}
}

void Entity::processnode(std::shared_ptr<Entity>who) {
	for (uint16 tmpI = 0; tmpI < this->children_.size(); tmpI++) {
		mat4 new_world = this->transform_ * children_[tmpI]->transform();

		children_[tmpI]->setWorld(new_world);

	}

	if (this->children_.size() > 0) {
		for (size_t i = 0; i < this->children_.size(); i++)
		{
			mat4 new_world = this->transform_ * this->children_[i]->transform();
			this->children_[i]->setWorld(new_world);
			if (this->children_[i]->children_.size() > 0) {
				//children_[i]->updateNodes();
			}
		}
	}

	if (who->getParent().get() != nullptr) {
		glm::mat4 model = who->updateModel(who->getPosition(), who->getRotation(), who->getScale());
		if (who->getParent().get() == nullptr) {
			who->setWorld(model);
		}
		else {
			glm::mat4 new_world = who->getParent()->transform()*model;
			who->setWorld(new_world);
		}
	}
}
void Entity::visitornode(std::shared_ptr<Entity> visitor) {
	processnode(visitor);
	if (children_.size() > 0) {
		std::vector<std::shared_ptr<Entity>> child = visitor->getChildren();

		for (unsigned int i = 0; i < child.size(); i++) {
			visitornode(child[i]);
		}
	}
}



std::vector<std::shared_ptr<Entity>> Entity::getChildren() {
	return children_;
}

void Entity::removeParent() {
	parent_ = nullptr;
}


void Entity::set_name(const string name) {
	name_ = name;
}
string Entity::name() const{
	return name_;
}

///CEREAL

void Entity::CerealLoad(cerealEntity ent) {
	/*position_ = ent.position;
	rotation_ = ent.rotation;
	scale_ = ent.scale;*/
	position(ent.position);
	rotation(ent.rotation);
	scale(ent.scale);
	set_name(ent.name_);
}