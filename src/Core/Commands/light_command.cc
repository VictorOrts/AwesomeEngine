#include "Core/Commands/light_command.h"
#include "Core/Light/spotlight.h"
#include "Core/Light/pointlight.h"
#include "Core/Light/dirlight.h"
#include "Core/OpenGL/aopengl.h"
#include "Core/engine.h"

using namespace Awesome;

DirLightCommand::DirLightCommand(Light* rLight)
{
	tmp = nullptr;
	tmp = rLight;
	program_ = Engine::engine_instance()->get_render()->program();
}

DirLightCommand::~DirLightCommand()
{
}
void DirLightCommand::execute() {
	AOpenGL::UseProgram(program_);
	DirLight * tmpRC = static_cast<DirLight*>(tmp);
	if (tmpRC->get_UniformsInfo().size() > 0) {
		for (auto& it : tmpRC->get_UniformsInfo()) {
			//Este sistema de comparacion cambiado de strings a uniforms 
			if (it.first == uniformLight::uType) {
				AOpenGL::setUniform(it.second, tmpRC->type_light());
			}
			if (it.first == uniformLight::uDirection) {
				AOpenGL::setUniform(it.second, tmp->direction());

			}
			if (it.first == uniformLight::uColor) {
				AOpenGL::setUniform(it.second, tmp->color().x, tmp->color().y, tmp->color().z);

			}
			if (it.first == uniformLight::uIntesity) {
				AOpenGL::setUniform(it.second, tmpRC->getIntesity());
			}
			if (it.first == uniformLight::uPosition) {
				AOpenGL::setUniform(it.second, tmpRC->position());
			}

			if (it.first == uniformLight::uLightSpaceMatrix) {
				AOpenGL::setUniform(it.second, tmpRC->getViewMatrixProjection());
			}
			else if (it.first == uniformLight::uShadowmap) {

				uint32 id_tex = tmpRC->get_Shadow()->get_texture();
				uint32 unit_tex = tmpRC->get_Shadow()->get_unit_texture();
				if(tmpRC->get_numberID()==0)
				{
					AOpenGL::setUniform(it.second, 2);
					AOpenGL::activeTexture(2);
				}else
				{
					AOpenGL::setUniform(it.second, 3);
					AOpenGL::activeTexture(3);
				}
				
				AOpenGL::bindTexture(id_tex, unit_tex);

			}
		}
	}
}