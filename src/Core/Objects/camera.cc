#include "Core/Objects/camera.h"
#include "Core\engine.h"
#include <deps/glm/gtc/matrix_transform.hpp>
#include <deps/glm//gtc/type_ptr.hpp>
using namespace Awesome;

Camera::Camera(vec3 position, vec2 nearfar, vec3 look, TypeCamera tc)
{
	camera_type_ = tc;
	position_ = position;
	rotation_ = vec2(0, 0);
	direction_ = vec3(0, 0, 1);
	near_far_ = nearfar;
	updateView();
	field_of_view_ = 45.0f;
	lookAt(look);
	updateView();
}

Camera::~Camera() {
}

void Camera::set_camera(vec3 position, float nearr, float farr) {
	/*near_far_ = vec2(nearr, farr);
	position_ = position;
	field_of_view_ = 45.0f;
	lookAt(vec3(0,0,0));*/
	updateView();
}
//////////////////////////////////////////////////////////////////////
mat4 Camera::view_matrix() {
	updateView();
	return view_;
}
mat4 Camera::projection_matrix() {
	switch (camera_type_)
	{
	case kPerspective:	projection_ = glm::perspective(glm::radians(field_of_view_), 800.0f / 600.0f, near_far_.x, near_far_.y);
		break;
	case kOrtho:		projection_ = glm::ortho(-15.0f, 15.0f, -15.0f, 15.0f, near_far_.x, near_far_.y);
		break;
	default:			projection_ = glm::perspective(glm::radians(field_of_view_), 800.0f / 600.0f, near_far_.x, near_far_.y);
		break;
	}
	return projection_;
}

mat4 Camera::view_projection_matrix() {
	return (projection_matrix()*view_matrix());
}

//////////////////////////////////////////////////////////////////////
void Camera::move_camera(CameraMove cmove, float time_x_spedd) {
	switch (cmove){
		case kForward: {
			setPosition(position_ + direction_*time_x_spedd);
		}
				  break;
		case kBack: {
			setPosition(position_ - direction_*time_x_spedd);
		}
					break;
		default: Engine::engine_instance()->add_log(string("WARNING::CAMERA::NO_MOVEMENT"), kLogColors::kColor_Yellow);
			break;
	}
}

//////////////////////////////////////////////////////////////////////

vec3 Camera::getPosition()  {
	return position_;
}

void Camera::setPosition( vec3 position) {
	position_ = position;

	updateView();
}

//////////////////////////////////////////////////////////////////////

vec2 Camera::getRotation()  {
	return rotation_;
}

void Camera::setRotation(vec2 rotation) {
	rotation_ = rotation;
	updateDirection();
	updateView();

}

void Camera::resetRotation() {
	rotation_ = vec2(0, 0);
}
//////////////////////////////////////////////////////////////////////

vec3 Camera::getDirection()  {
	return direction_;
}

void Camera::setDirection( vec3 direction) {
	direction_ = direction;

	this->updateRotation();
	this->updateView();
}

//////////////////////////////////////////////////////////////////////

void Camera::lookAt(vec3 point) {
	//point
	view_ = glm::lookAt(position_, point, vec3(0, 1, 0));
	direction_ = glm::normalize(point - position_);
	this->updateRotation();
	this->updateView();
	//Añadido esta linea para aquellas camaras que empiezan rotadas se vean bien y no haya que moverlas
	this->updateDirection();
}

//////////////////////////////////////////////////////////////////////

void Camera::setNearFar(float n, float f) {
	near_far_.x = n;
	near_far_.y = f;
}
float Camera::getNear() const {
	return near_far_.x;
}
float Camera::getFar() const {
	return near_far_.y;
}


/////////////////////////////////////////////////////////////////////

void Camera::updateView() {
	view_ = glm::lookAt(position_, position_ + direction_, vec3(0, 1, 0));
}

void Camera::updateDirection() {
	if (rotation_.y > horizontalAngle) rotation_.y -= horizontalAngle;
	if (rotation_.y < 0) rotation_.y += horizontalAngle;
	if (rotation_.x > verticalAngle) rotation_.x = verticalAngle;
	if (rotation_.x < -verticalAngle) rotation_.x = -verticalAngle;

	vec2 SinCos_x = vec2(sin(glm::radians(rotation_.x)), cos(glm::radians(rotation_.x)));
	vec2 SinCos_y = vec2(sin(glm::radians(rotation_.y)), cos(glm::radians(rotation_.y)));
	direction_ = vec3(SinCos_x.y * SinCos_y.x, SinCos_x.x, SinCos_x.y * SinCos_y.y);
}

void Camera::updateRotation() {

	float distXZ = glm::sqrt(direction_.x * direction_.x + direction_.z * direction_.z);
	float distXYZ = glm::sqrt(direction_.x * direction_.x + direction_.y * direction_.y + direction_.z * direction_.z);

	if (direction_.x != 0 || direction_.z != 0) {
		rotation_.y = glm::degrees(acos(direction_.z / distXZ));
	}

	if (direction_.x != 0 || direction_.y != 0 || direction_.z != 0) {
		rotation_.x = -glm::degrees(acos(distXZ / distXYZ));
	}

	if (direction_.x > 0) {
		rotation_.y = -rotation_.y;
	}

	if (direction_.y > 0) {
		rotation_.x = -rotation_.x;
	}

}

Camera::TypeCamera Camera::getTypecamera() const 
{
	return camera_type_;
}
