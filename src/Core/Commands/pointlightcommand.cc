#include "Core/Commands/pointlightcommand.h"
#include "Core/engine.h"
#include "Core/OpenGL/aopengl.h"
using namespace Awesome;

PointLightCommand::PointLightCommand(PointLight * rlight) {
	pl = rlight;
	program_ = Engine::engine_instance()->get_render()->program();
}
PointLightCommand::~PointLightCommand() {

}
void PointLightCommand::execute() {
	AOpenGL::UseProgram(program_);
	
	if (pl->get_UniformsInfo().size() > 0) {
		for (auto& it : pl->get_UniformsInfo()) {
			if (it.first == "mylight.type") {
				AOpenGL::setUniform(it.second, pl->type_light());
			}
			if (it.first == "mylight.direction") {
				AOpenGL::setUniform(it.second, pl->direction());

			}
			if (it.first == "mylight.color") {
				AOpenGL::setUniform(it.second, pl->color());

			}
			if (it.first == "mylight.position") {
				AOpenGL::setUniform(it.second, pl->position());
			}
		}
	}
}