#include "Core/Components/lightcomponent.h"
#include "Core/engine.h"
#include "Core/Light/dirlight.h"

using namespace Awesome;
LightComponent::LightComponent()
{
	//Engine::engine_instance()->currentScene()->addLightC(this);
	name_ = "Light" + std::to_string(Engine::engine_instance()->currentScene()->get_lights().size());
}

LightComponent::~LightComponent()
{
}

void LightComponent::update() {
}
void LightComponent::render(mat4 transform, uint32 program) {
	if (light_ != nullptr && game_object() != nullptr) {
		if (light_->position() != go_->getPosition()) {
			light_->set_position(go_->getPosition());
		}
		//light_->update();
	}
	//std::shared_ptr<Command>
	/*if (light_ != nullptr) {
		switch (light_->type_light())
		{
		case Awesome::kDirLight:
			printf("\nDirectionalLight");
			break;
		case Awesome::kPointLight:
			printf("\nPointLight");
			break;
		case Awesome::kSpotLight:
			printf("\nSpotLight");
			break;
		default: printf("\nEROOR::LIGHT");
			break;
		}
	}*/
}

void LightComponent::addLight(std::shared_ptr<Light> a) {
	light_ = a;
}

std::shared_ptr<Light> LightComponent::getLight() {
	return light_;
}