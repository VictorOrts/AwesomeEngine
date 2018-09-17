#include "Core/Light/spotlight.h"
#include "Core/Commands/spotlightcommand.h"
#include "Core/OpenGL/aopengl.h"
#include "Core/engine.h"
using namespace Awesome;

SpotLight::SpotLight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection, float cutOff, float outerCutOff, float constant, float linear, float quadratic) {
	set_type_light(kSpotLight);
	set_position(lightPosition);
	set_direction(lightDirection);
	set_color(lightColor);
	cutOff_ = cutOff;
	outerCutOff_ = outerCutOff;
	constant_ = constant;
	linear_ = linear;
	quadratic_ = quadratic;
	setUniformInfo();
	cast_shadow_ = true;
	lightCamera_ = std::make_shared<Camera>(lightPosition, vec2(1.0f, 100.0f), vec3(0), Camera::kOrtho);
}

SpotLight::~SpotLight()
{
}
void SpotLight::setUniformInfo() {
	Engine::engine_instance()->get_render()->SetRenderMaterial(kNormal);
	uint32 program_ = Engine::engine_instance()->get_render()->program();
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uType, AOpenGL::uniform_location(program_, "mylight.type")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uDirection, AOpenGL::uniform_location(program_, "mylight.direction")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uPosition, AOpenGL::uniform_location(program_, "mylight.position")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uColor, AOpenGL::uniform_location(program_, "mylight.color")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uConstant, AOpenGL::uniform_location(program_, "mylight.constant")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLinear, AOpenGL::uniform_location(program_, "mylight.linear")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uQuadratic, AOpenGL::uniform_location(program_, "mylight.quadratic")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uCutoff, AOpenGL::uniform_location(program_, "mylight.cutOff")));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uOuterCuttof, AOpenGL::uniform_location(program_, "mylight.outerCutOff")));


	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uType,
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].type").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uPosition,
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].position").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uDirection,
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].direction").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uColor,
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].color").c_str())));

	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uConstant, 
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].constant").c_str())));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLinear, 
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].linear").c_str())));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uQuadratic, 
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].quadratic").c_str())));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uCutoff, 
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].cutOff").c_str())));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uOuterCuttof, 
		AOpenGL::uniform_location(program_, string("mylight[" + std::to_string(numberID_) + "].outerCutOff").c_str())));

	//Otras cosas necesarias para el material light
	/*uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLightPos,
	AOpenGL::uniform_location(program_, "lightPos")));*/
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uLightSpaceMatrix,
		AOpenGL::uniform_location(program_,
			string("lightSpaceMatrix[" + std::to_string(numberID_) + "]").c_str())));
	uniform_info.insert(std::pair<int8, uint32>(uniformLight::uShadowmap,
		AOpenGL::uniform_location(program_, string("shadowMap[" + std::to_string(numberID_) + "]").c_str())));
}

void SpotLight::update(int8 id) {
	std::shared_ptr<SpotLightCommand> LightC = std::make_shared<SpotLightCommand>(this);
	Engine::engine_instance()->get_render()->get_display()->addCommand(LightC);
}
uniform_faster_map SpotLight::get_UniformsInfo(){
	return uniform_info;
}
float SpotLight::getCutOff() {
	return cutOff_;
}
float SpotLight::getOuterCutOff() {
	return outerCutOff_;
}
float SpotLight::getConstant() {
	return constant_;
}
float SpotLight::getLinear() {
	return linear_;
}
float SpotLight::getQuadratic() {
	return quadratic_;
}