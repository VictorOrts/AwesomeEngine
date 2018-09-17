#include "Core\material.h"
#include "Core\OpenGL\aopengl.h"
#include "Core\engine.h"
using namespace Awesome;

Material::Material() {
	uniforms_ = AOpenGL::getUniforms(Engine::engine_instance()->get_render()->program());
	passUniformsToFasterMethod();
	Ambient = glm::vec3(1, 1, 1);
	Diffuse = glm::vec3(1, 1, 1);
	Specular = glm::vec3(1, 1, 1);
}

Material::~Material() {
}

void Material::addTexture(std::shared_ptr<Texture> tex_) {
	texture_.push_back(tex_);
}
std::vector<std::shared_ptr<Texture>> Material::textures() {
	return texture_;
}

void Material::passUniformsToFasterMethod() {
	for (auto& it : uniforms_) {
		if (it.first == "diffuse_tex") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uDiffuse_tex, it.second));
		}
		else if (it.first == "specular_tex") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uSpecular_tex, it.second));
		}
		else if (it.first == "currentLight") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uCurrentLight, it.second));
		}
		else if (it.first == "model") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uModel, it.second));
		}
		else if (it.first == "viewproject") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uViewproject, it.second));
		}
		else if (it.first == "CameraPos") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uCameraPos, it.second));
		}
		else if (it.first == "mat.ambient") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uMatAmbient, it.second));
		}
		else if (it.first == "mat.diffuse") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uMatDiffuse, it.second));
		}
		else if (it.first == "mat.specular") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uMatSpecular, it.second));
		}
		else if (it.first == "mat.shininess") {
			uniforms_information_.insert(std::pair<int8, uint32>(uniformEntity::uMatShininess, it.second));
		}
	}
}