#include "Core/Commands/program_command.h"
#include "Core/OpenGL/aopengl.h"
#include "deps/glm/gtc/matrix_transform.hpp"
#include "Core/engine.h"
using namespace Awesome;
ProgramCommand::ProgramCommand(uint32 program,Material* r_mat, mat4 mat,mat4 camera)
{
	este = r_mat;
	program_ = program;
	model_ = mat;
	proyection_view_ = camera;
}

ProgramCommand::~ProgramCommand()
{
}

void ProgramCommand::execute() {
	//printf("\nProgram using %d", program_);
	AOpenGL::UseProgram(program_);
	//uniform_map::iterator it = este->get_uniforms_information().begin();
	if (program_ == Engine::engine_instance()->get_render()->program(RenderMaterial::kDepth)){
		//Utils::Log("ProgramDepth", kLogColors::kColor_Green);
		//AOpenGL::setUniform(AOpenGL::uniform_location(program_, "viewproject"), proyection_view_);
		AOpenGL::setUniform(AOpenGL::uniform_location(program_, "model"), model_);
		AOpenGL::setUniform(AOpenGL::uniform_location(program_, "lightSpaceMatrix"),proyection_view_ /*Engine::engine_instance()->currentScene()->get_lights()[0]->getViewMatrixProjection()*/);

	}
	else {
		if (este->get_uniforms_information().size() > 0) {
			for (auto& it : este->get_uniforms_information()) {
				//std::cout << kv.first << " has value " << kv.second << std::endl;
				if (it.first == uniformEntity::uDiffuse_tex ) {
					uint32 id_tex = este->textures()[0]->id_texture();
					uint32 unit_tex = este->textures()[0]->texture_unit();
					AOpenGL::setUniform(it.second, 0);
					AOpenGL::activeTexture(0);
					AOpenGL::bindTexture(id_tex,este->textures()[0]->texture_unit());
				}
				else if (it.first == uniformEntity::uSpecular_tex) {
					if (este->textures().size() > 1) {
						uint32 id_tex = este->textures()[1]->id_texture();
						uint32 unit_tex = este->textures()[1]->texture_unit();
						AOpenGL::setUniform(it.second, 1);
						AOpenGL::activeTexture(1);
						AOpenGL::bindTexture(id_tex, este->textures()[1]->texture_unit());
					}
				}
				else if (it.first == uniformEntity::uCurrentLight) {
					AOpenGL::setUniform(it.second, Engine::engine_instance()->currentScene()->get_number_light());
				}
				else if (it.first == uniformEntity::uModel) {
					AOpenGL::setUniform(it.second, model_);
				}
				else if (it.first == uniformEntity::uViewproject) {
					AOpenGL::setUniform(it.second, proyection_view_);
				}
				else if (it.first == uniformEntity::uViewproject) {
					vec3 position = Engine::engine_instance()->currentScene()->currentCamera()->getPosition();
					AOpenGL::setUniform(it.second, position.x, position.y, position.z);
				}
				else if (it.first == uniformEntity::uMatAmbient) {
					AOpenGL::setUniform(it.second, este->Ambient);
				}
				else if (it.first == uniformEntity::uMatDiffuse) {
					AOpenGL::setUniform(it.second, este->Diffuse);
				}
				else if (it.first == uniformEntity::uMatSpecular) {
					AOpenGL::setUniform(it.second, este->Specular);
				}
				else if (it.first == uniformEntity::uMatShininess) {
					AOpenGL::setUniform(it.second, este->Shininess);
				}
			}
			//End for
		}
	}
}