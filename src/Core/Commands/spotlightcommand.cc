#include "Core/Commands/spotlightcommand.h"
#include "Core/engine.h"
#include "Core/OpenGL/aopengl.h"

using namespace Awesome;

SpotLightCommand::SpotLightCommand(SpotLight* spot)
{
	spl = spot;
	program_ = Engine::engine_instance()->get_render()->program();
}

SpotLightCommand::~SpotLightCommand()
{
}

void SpotLightCommand::execute() {
	AOpenGL::UseProgram(program_);
	if (spl->get_UniformsInfo().size() > 0) {
		for (auto& it : spl->get_UniformsInfo()) {
			if (it.first == uniformLight::uType) {
				AOpenGL::setUniform(it.second, spl->type_light());
			}
			if (it.first == uniformLight::uDirection) {
				AOpenGL::setUniform(it.second, spl->direction());

			}
			if (it.first == uniformLight::uColor) {
				AOpenGL::setUniform(it.second, spl->color());

			}
			if (it.first == uniformLight::uPosition) {
				AOpenGL::setUniform(it.second, spl->position());
			}
			if (it.first == uniformLight::uConstant) {
				AOpenGL::setUniform(it.second, spl->getConstant());
			}
			if (it.first == uniformLight::uLinear) {
				AOpenGL::setUniform(it.second, spl->getLinear());
			}
			if (it.first == uniformLight::uLinear) {
				AOpenGL::setUniform(it.second, spl->getLinear());
			}
			if (it.first == uniformLight::uCutoff) {
				AOpenGL::setUniform(it.second, glm::cos(glm::radians(spl->getCutOff())));
			}
			if (it.first == uniformLight::uOuterCuttof) {
				AOpenGL::setUniform(it.second, glm::cos(glm::radians(spl->getOuterCutOff())));
			}
			if (it.first == uniformLight::uLightPos) {
				AOpenGL::setUniform(it.second, spl->position());
			}
			if (it.first == uniformLight::uLightSpaceMatrix) {
				AOpenGL::setUniform(it.second, spl->getViewMatrixProjection());
			}
			if (it.first == uniformLight::uShadowmap) {

				uint32 id_tex = spl->get_Shadow()->get_texture();
				uint32 unit_tex = spl->get_Shadow()->get_unit_texture();
				AOpenGL::setUniform(it.second, 2);
				AOpenGL::activeTexture(2);
				AOpenGL::bindTexture(id_tex, unit_tex);

			}
		}
	}
}