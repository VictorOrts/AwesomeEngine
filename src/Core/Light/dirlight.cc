#include "Core/Light/dirlight.h"
#include "Core/Commands/light_command.h"
#include "Core/engine.h"
#include "Core/OpenGL/aopengl.h"
using namespace Awesome;

DirLight::DirLight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection, float intensity){
	set_position(lightPosition);
	set_direction(lightDirection);
	set_color(lightColor);
	setIntesity(intensity);
	set_type_light(kDirLight);
	//setUniformInfo();
	cast_shadow_ = true;
	lightCamera_ = std::make_shared<Camera>(lightPosition, vec2(1.0f, 100.0f), vec3(0), Camera::kOrtho);
	
}

DirLight::~DirLight() {
}

void DirLight::Init()
{
	name_ = "Directioanl" + std::to_string(get_numberID());
	setUniformInfo();
}
void DirLight::setUniformInfo() {
	Engine::engine_instance()->get_render()->SetRenderMaterial(kNormal);
	uint32 program_ = Engine::engine_instance()->get_render()->program();
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uType, 
		AOpenGL::uniform_location(program_, string("mylight["+std::to_string(numberID_)+"].type").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uPosition,
		AOpenGL::uniform_location(program_, string("mylight["+std::to_string(numberID_)+"].position").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uDirection,
		AOpenGL::uniform_location(program_, string("mylight["+std::to_string(numberID_)+"].direction").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uColor,
		AOpenGL::uniform_location(program_, string("mylight["+std::to_string(numberID_)+"].color").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uIntesity,
		AOpenGL::uniform_location(program_, string("mylight["+std::to_string(numberID_)+"].intesity").c_str())));
	//Otras cosas necesarias para el material light
	/*uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLightPos,
		AOpenGL::uniform_location(program_, "lightPos")));*/
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLightSpaceMatrix,
		AOpenGL::uniform_location(program_, 
			string("lightSpaceMatrix[" + std::to_string(numberID_) + "]").c_str())));
	if (get_numberID() == 0) {
		uniform_info.insert(std::pair<int8, uint32>(uniformLight::uShadowmap,
			AOpenGL::uniform_location(program_, "shadowMap")));
	}else
	{
		uniform_info.insert(std::pair<int8, uint32>(uniformLight::uShadowmap,
			AOpenGL::uniform_location(program_, "shadowMap2")));
	}
}

float DirLight::getIntesity() const{
	return intensity_;
}
void DirLight::setIntesity(float intes) {
	intensity_ = intes;
}

void DirLight::update(int8 id) {
	numberID_ = id;
	//printf("\nPosition (%f,%f,%f)", position().x, position().y, position().z);
	std::shared_ptr<DirLightCommand> LightC = std::make_shared<DirLightCommand>(this);
	Engine::engine_instance()->get_render()->get_display()->addCommand(LightC);
}

uniform_faster_map DirLight::get_UniformsInfo() const {
	return uniform_info;
}