#include "Core/scene.h"
#include "Core/engine.h"
#include "Core/Task/displaytask.h"
#include "Core/Light/dirlight.h"
using namespace Awesome;

Scene::Scene() {

	current_camera_ = nullptr;
	rtt_ = std::make_shared<RenderToTexture>(800,600);
	name_ = "Scene";
	entityCenter_ = std::make_shared<EntityManager>();
	count_Light = 0;
	freichen = -1;
	gradient = -1;
}

Scene::~Scene() {
}

void Scene::update() {
	for (uint32 i = 0; i < entityCenter_->getList().size(); i++) {
		//gameobjects_array_[i]->update();
		entityCenter_->getList()[i]->update();
	}
	Engine::engine_instance()->get_task()->AddTask(std::make_shared<DisplayTask>(this));
}
void Scene::render() {
	
	Engine::engine_instance()->get_render()->run();
}

void Scene::setName(string name) {
	name_ = name;
}
string Scene::name_scene() {
	return name_;
}
template <class tmpObject>
std::shared_ptr<tmpObject> Scene::CreateObject() {
	std::shared_ptr<tmpObject> tmpEntity = make_shared<tmpObject>();
	return tmpEntity;
}
//LIGHT CREATOR TEMPLATES

std::shared_ptr<Texture> Scene::CreateTexture(string name_) {
	return std::make_shared<Texture>(name_);
}

std::shared_ptr<Camera> Scene::CreateCamera(vec3 position, vec2 near_far, vec3 lookAt, Camera::TypeCamera typecamera) {
	std::shared_ptr<Camera> tmpCamera = std::make_shared<Camera>(position, near_far, lookAt, typecamera);
	addCamera(tmpCamera);
	return tmpCamera;
}

std::vector<std::shared_ptr<GameObject>> Scene::get_gameobjects() {
	return gameobjects_array_;
}

std::vector<std::shared_ptr<Camera>> Scene::camera() {
	return camera_;
}

std::shared_ptr<Camera> Scene::currentCamera() {
	return current_camera_;
}

void Scene::addCamera(std::shared_ptr<Camera> cam) {
	if (camera_.size() == 0) {
		current_camera_ = cam;
	}
	camera_.push_back(cam);
}

void Scene::setCamera(uint32 cam_select) {
	if (cam_select >= 0 && cam_select < camera_.size()) {
		current_camera_ = camera_[cam_select];
	}
	else {
		Utils::Log(string("ERROR::SCENE::DONT_EXIST_THIS_CAMERA " + std::to_string(camera_.size()) + std::to_string(cam_select)), kLogColors::kColor_Red);
	}
}

void Scene::addLightC(std::shared_ptr<Light>a) {
	if (light_array_.size() < MAX_LIGHT) {
		a->set_numberID_light(count_Light);
		a->Init();
		light_array_.push_back(a);
		count_Light++;
	}
	else {
		Utils::Log("WARNING::SCENE::LIGHT ARRAY FULL", kLogColors::kColor_Yellow);
	}
}

int8 Scene::get_number_light() {
	return count_Light;
}

std::shared_ptr<RenderToTexture> Scene::get_rtt() {
	return rtt_;
}

std::shared_ptr<EntityManager> Scene::getEntityManager()
{
	return entityCenter_;
}
std::shared_ptr<Entity> Scene::CreateEntity() {
	return entityCenter_->Create();
}


std::vector<std::shared_ptr<Light>> Scene::get_lights() {
	return light_array_;
}
