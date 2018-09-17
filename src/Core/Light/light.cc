#include "Core/Light/light.h"

using namespace Awesome;

Light::Light() {
	typeL = LightType::kNone;
	cast_shadow_ = true;
	shadow_ = std::make_shared<ShadowTexture>(1024, 1024);
	numberID_ = 0;
}

Light::~Light() {

}

bool Light::get_cast_shadows() const {
	return cast_shadow_;
}

void Light::set_cast_shadows(bool active) {
	cast_shadow_ = active;
}
LightType Light::type_light() const{
	return typeL;
}

void Light::set_position(vec3 pos) {
	position_ = pos;
	if (lightCamera_ != nullptr) {
		lightCamera_->setPosition(pos);
	}
}

vec3 Light::position() {
	return position_;
}

void Light::set_direction(vec3 dir_) {
	direction_ = dir_;
}

vec3 Light::direction() {
	return direction_;
}

vec3 Light::color() {
	return light_color_;
}
void Light::set_color(vec3 col) {
	//Debug for color when corvese /255 or /255 not fixed 
	//printf("\nColor %f %f %f", col.x, col.y, col.z);
	light_color_ = col;
}

std::shared_ptr<ShadowTexture> Light::get_Shadow() const{
	return shadow_;
}

mat4 Light::getViewMatrixProjection() {
	return lightCamera_->view_projection_matrix();
}

void Light::set_numberID_light(int8 number) {
	numberID_ = number;
}
int8 Light::get_numberID() {
	return numberID_;
}
string Light::get_name()
{
	return name_;
}