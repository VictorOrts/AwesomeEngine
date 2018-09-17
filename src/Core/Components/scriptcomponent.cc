#include "Core/Components/scriptcomponent.h"
#include "Core/Components/rendercomponent.h"
#include "Core/Geometry/cube.h"
#include "Core/material.h"
#include "Core/engine.h"
using namespace Awesome;

struct ScriptComponent::ScriptData {
	chaiscript::ChaiScript chai_ =(chaiscript::Std_Lib::library());
	ScriptData() {
		
	}
};

ScriptComponent::ScriptComponent() {
	type_component_ = kScriptComponent;
	enabled_ = true;
	data_ptr_ = new ScriptData();
	scriptInit();
}

ScriptComponent::~ScriptComponent() {
}



void ScriptComponent::set_file(string file) {
	fileName_ = file;
	//data_ptr_->chai_(chaiscript::Std_Lib::library());
	//chai_->eval_file("script.chai");
	data_ptr_->chai_.eval_file("script.chai");
	onStart_ = data_ptr_->chai_.eval<std::function<void()>>("start");
	onUpdate_ = data_ptr_->chai_.eval<std::function<void()>>("update");

	onStart_();
}

void ScriptComponent::update() {
	//onUpdate_ = data_ptr_->chai_.eval<std::function<void()>>("update");
	onUpdate_();
}

void ScriptComponent::render(mat4 transform, uint32 program) {
	//Se pone esto aqui en vez del constructor porque en constructor esta los gameobjects vacios.

}

void ScriptComponent::scriptInit() {
	data_ptr_->chai_.add(chaiscript::user_type<Entity>(), "GO");
	data_ptr_->chai_.add(chaiscript::base_class<std::enable_shared_from_this<Entity>, Entity>());
	//data_ptr_->chai_.add(chaiscript::fun(&Entity::setName), "setName");
	data_ptr_->chai_.add(chaiscript::fun<void, Entity, const float, const float, const float>(&Entity::scale), "scale");
	data_ptr_->chai_.add(chaiscript::fun<void, Entity, const float, const float, const float>(&Entity::move), "move");
	data_ptr_->chai_.add(chaiscript::fun<void, Entity, const float, const float, const float>(&Entity::rotate), "rotate");
	data_ptr_->chai_.add(chaiscript::fun(&ScriptComponent::getGameObject, this), "getGameObject");
}


std::shared_ptr<Entity> ScriptComponent::getGameObject(const std::string& name) {
	//printf("\n%s Size %d\n", name.c_str(), Engine::engine_instance()->currentScene()->get_gameobjects().size());
	for (uint32 i = 0; i < Engine::engine_instance()->currentScene()->getEntityManager()->getList().size(); i++) {
		if (!Engine::engine_instance()->currentScene()->getEntityManager()->getList()[i]->name().compare(name)) {
			//printf("\n%s %s", Engine::engine_instance()->currentScene()->get_gameobjects()[i]->name().c_str(), name.c_str());
			Utils::Log(name, kLogColors::kColor_Red, kLogOutput::kWindow_CMD);
			return Engine::engine_instance()->currentScene()->getEntityManager()->getList()[i];
		}
	}
	Utils::Log("No se encontro nada", kLogColors::kColor_Red, kLogOutput::kWindow_CMD);
	return nullptr;
}

