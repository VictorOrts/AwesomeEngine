#include "Core/Components/SineTransform.h"
#include "Core/Objects/entity.h"
#include "Core/engine.h"
using namespace Awesome;

SineTransform::SineTransform() {
	type_component_ = KSineTransform;
}
SineTransform::~SineTransform() {
}


void SineTransform::setTypeAxix(SineAxis a) {
	valueSine = a;
}
void SineTransform::update() {
	vec3 position = game_object()->getPosition();
	float timeTo = Engine::engine_instance()->get_time()->time_elapsed();
	if (valueSine == SineAxis::Sine_X) {
		position.x = 10 * sin(5 * timeTo);
	}
	else if (valueSine == SineAxis::Sine_y){
		position.y = 10 * sin(5 * timeTo);
	}
	else if (valueSine == SineAxis::Sine_Z) {
		position.z = 10 * sin(5 * timeTo);
	}
	game_object()->position(position);
}
void SineTransform::render(mat4 transform, uint32 program) {

}