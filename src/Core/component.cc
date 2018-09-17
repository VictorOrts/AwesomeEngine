#include "Core/component.h"
#include "Core/Objects/entity.h"
using namespace Awesome;

Component::Component()
{
}

Component::~Component() {
}

ComponentType Component::get_type_component() const{
	return type_component_;
}

void Component::set_game_object(std::shared_ptr<Entity> rgo) {
	go_ = rgo;
}

std::shared_ptr<Entity> Component::game_object() {
	return go_;
}

vec3 Component::position() {
	return go_->getPosition();
}