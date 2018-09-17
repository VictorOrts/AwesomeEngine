#include "Core/Light/pointlight.h"
#include "Core/engine.h"
#include "Core/OpenGL/aopengl.h"
#include "Core/Commands/pointlightcommand.h"
using namespace Awesome;

PointLight::PointLight(glm::vec3 lightColor, glm::vec3 lightPosition, float constant, float linear, float quadratic)
{
	set_position(lightPosition);
	set_color(lightColor);
	constant_ = constant;
	linear_ = linear;
	quadratic_ = quadratic;
	set_type_light( kPointLight);
}

PointLight::~PointLight()
{
}


void PointLight::update(int8 id){
	std::shared_ptr<PointLightCommand> LightC = std::make_shared<PointLightCommand>(this);
	Engine::engine_instance()->get_render()->get_display()->addCommand(LightC);
}

float PointLight::getConstant() {
	return constant_;
}
float PointLight::getLinear() {
	return linear_;
}
float PointLight::getQuadratic() {
	return quadratic_;
}

uniform_map PointLight::get_UniformsInfo() {
	return uniform_info;
}

void PointLight::setUniformInfo() {
	Engine::engine_instance()->get_render()->SetRenderMaterial(kNormal);
	uint32 program_ = Engine::engine_instance()->get_render()->program();
	uniform_info.insert(std::pair<string, uint32>(string("mylight.type"), AOpenGL::uniform_location(program_, "mylight.type")));
	uniform_info.insert(std::pair<string, uint32>(string("mylight.color"), AOpenGL::uniform_location(program_, "mylight.color")));
	uniform_info.insert(std::pair<string, uint32>(string("mylight.constant"), AOpenGL::uniform_location(program_, "mylight.constant")));
	uniform_info.insert(std::pair<string, uint32>(string("mylight.linear"), AOpenGL::uniform_location(program_, "mylight.linear")));
	uniform_info.insert(std::pair<string, uint32>(string("mylight.quadratic"), AOpenGL::uniform_location(program_, "mylight.quadratic")));

}