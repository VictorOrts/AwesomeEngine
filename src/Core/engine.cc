#include "Core/engine.h"
#include "Core/Objects/mycerealsaves.h"
#include "Core/Components/rendercomponent.h"
using namespace Awesome;
Engine* Engine::m_pIEngine = nullptr;

Engine::Engine() {
	currentScene_ = nullptr;
	Utils::Log("Init Engine -> AwesomeEngine", kLogColors::kColor_Default);
}

Engine::~Engine() {
	delete m_pIEngine;
	m_pIEngine = nullptr;
}
Engine *Engine::engine_instance() {
	if (m_pIEngine == nullptr) {
		m_pIEngine = new Engine;
		Utils::Log("Create Engine", kLogColors::kColor_Default);
	}
	return m_pIEngine;
}
void Engine::initEngine(string a, Window::WindowResolution res) {
	window_ = std::make_shared<Window>();
	window_->init(a.c_str(), Window::WindowResolution(res));
	render_ = std::make_shared<Render>();
	taskM_ = std::make_shared<TaskManager>();
	input_ = std::make_shared<Input>();
	time_ = std::make_shared<Time>();
	
	
	gui_ = std::make_shared<Gui>();
	defaults_ = std::make_shared<DefaultObjects>();
	defaults_->init();
}

void Engine::setScene(std::shared_ptr<Scene> scene) {
	currentScene_ = scene;
}

std::shared_ptr<Scene> Engine::currentScene() const{
	return currentScene_;
}

void Engine::run() {
	
	render();
	update();
}
std::shared_ptr<Window> Engine::window() {
	return window_;
}

void Engine::reset_instance() {
	delete m_pIEngine;
	m_pIEngine = nullptr;
}

void Engine::add_log(string log, kLogColors typelog) {
	ivec3 color;
	switch (typelog)
	{
	case kLogColors::kColor_Default: color = ivec3(255, 255, 255);
		break;
	case kLogColors::kColor_Green: color = ivec3(0, 255, 0);
		break;
	case kLogColors::kColor_Yellow: color = ivec3(255, 255, 0);
		break;
	case kLogColors::kColor_Red: color = ivec3(255, 0, 0);
		break;
	default:
		break;
	}
	
	//Old method add everything
	//logArray_.push_back(tmp);
	//if is empty add the element
	if (logArray_.size() <= 0) {
//uniform_info.insert(std::pair<string, uint32>(string("mylight.type"), AOpenGL::uniform_location(program_, "mylight.type")));
		logArray_.insert(std::pair<string, ivec3>(log, color));
	}
	else {
		//If this value change to 0 dont add to vector
		int counter = 1;
		for (auto& it : logArray_) {
			if (it.first.compare(log) != 0 && counter == 1) {
				//printf("\nSon diferentes");
				//printf("Compare %s -- %s", logArray_[i].log_data.c_str(), tmp.log_data.c_str());
				counter = 1;
				//logArray_.push_back(tmp);
			}
			else {
				//Should made a break ? for dont search in the rest of the vector
				counter = 0;
				break;
				//printf("Son Iguales");
				//logArray_.push_back(tmp);
			}
		}
		if (counter == 1) {
			logArray_.insert(std::pair<string, ivec3>(log, color));
		}
	}
}
logGui Engine::log_array() {
	return logArray_;
}

void Engine::clear_log() {
	logArray_.clear();
}

std::shared_ptr<Input> Engine::get_input() {
	return input_;
}

void Engine::update() {
	currentScene_->update();
}

void Engine::render() {
	gui_->new_frame();
	currentScene_->render();
	gui_->draw();
}



std::shared_ptr<DefaultObjects> Engine::defaults() {
	return defaults_;
}

std::shared_ptr<Render> Engine::get_render() {
	return render_;
}
std::shared_ptr<TaskManager> Engine::get_task() {
	return taskM_;
}

void Engine::closeEngine() {
	taskM_->JoinAll(true);
	window_->finish();
}



std::shared_ptr<Time> Engine::get_time() const {
	return time_;
}

//LOAD AND SAVE

void Engine::SaveScene(const string& nFile) const {
	std::ofstream os(nFile.c_str(), std::ios::binary);
	cereal::JSONOutputArchive archive(os);
	std::vector<cerealEntity> myCerealEntity;
	std::vector<cerealCamera> myCerealCamera;
	std::vector<cerealDirectionalLight> mycerealDirLight;
	for (uint8 i = 0; i<currentScene_->getEntityManager()->getList().size(); i++) {
		cerealEntity tmpCereal;
		tmpCereal.SetEntity(*currentScene_->getEntityManager()->getList()[i]);
		myCerealEntity.push_back(tmpCereal);
	}
	for (uint8 i = 0; i < currentScene_->camera().size();i++) {
		cerealCamera tmpCamera;
		tmpCamera.SetCamera(*currentScene_->camera()[i]);
		myCerealCamera.push_back(tmpCamera);
	}
	for(uint8 i = 0;i<currentScene_->get_lights().size();i++)
	{
		cerealDirectionalLight tmpDirLight;
		
		if(currentScene_->get_lights()[i]->type_light()==LightType::kDirLight)
		{
			DirLight * tmpDir = (DirLight*)currentScene_->get_lights()[i].get();
			tmpDirLight.SetDirectional(*tmpDir);
			mycerealDirLight.push_back(tmpDirLight);
		}
		
	}
	archive(cereal::make_nvp("Entitys", myCerealEntity), cereal::make_nvp("Cameras", myCerealCamera), cereal::make_nvp("DirectionalLight", mycerealDirLight));
}

void Engine::LoadScene(const string& nFile) const{

	std::ifstream is(nFile.c_str());
	cereal::JSONInputArchive archive(is);
	std::vector<cerealEntity> myCerealEntity;
	std::vector<cerealCamera> myCerealCamera;
	std::vector<cerealDirectionalLight> mycerealDirLight;
	archive(myCerealEntity,myCerealCamera,mycerealDirLight);
	std::shared_ptr<Texture> tex = currentScene_->CreateTexture("woodDiff.png");
	std::shared_ptr<Texture> texspec = currentScene_->CreateTexture("woodSpec.png");
	
	for (uint8 i = 0; i < myCerealEntity.size(); i++) {
		auto tmpEntity = currentScene_->CreateEntity();
		tmpEntity->CerealLoad(myCerealEntity[i]);
		tmpEntity->addComponent<RenderComponent>();
		tmpEntity->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMesh<Cube>();
		tmpEntity->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMaterial();
		tmpEntity->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(tex);
		tmpEntity->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(texspec);
	}

	for (uint8 i = 0; i < mycerealDirLight.size(); i++) {
		auto tmpLight = std::make_shared<DirLight>(mycerealDirLight[i].color, mycerealDirLight[i].position, mycerealDirLight[i].direction, mycerealDirLight[i].intesity);
		currentScene_->addLightC(tmpLight);
	}
	for (uint8 i = 0; i < myCerealCamera.size(); i++) {
		auto tmpEntity = currentScene_->CreateCamera(myCerealCamera[i].position, myCerealCamera[i].nearfar,vec3(0,0,0),(Camera::TypeCamera)myCerealCamera[i].typeCamera);

	}
}