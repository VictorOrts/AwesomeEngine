

void example_2B(std::shared_ptr<Scene> BasicScene);
void example_cubes(std::shared_ptr<Scene> BasicScene);

void example_2B(std::shared_ptr<Scene> BasicScene) {


	std::shared_ptr<Camera> cam = BasicScene->CreateCamera(vec3(0, 10, 10), vec2(0.1f, 200.0f), vec3(0), Camera::kPerspective);

	//std::shared_ptr<Camera> cam2 = BasicScene->CreateCamera(vec3(0, 51, -59), vec2(1.0f, 100.0f), vec3(0), Camera::kOrtho);

	std::shared_ptr<Obj> cube_ = std::make_shared<Obj>("2b.obj");
	cube_->Start();
	std::shared_ptr<Texture> tex = BasicScene->CreateTexture("woodDiff.png");
	std::shared_ptr<Texture> texspec = BasicScene->CreateTexture("woodSpec.png");
	std::shared_ptr<Texture> tex2b = BasicScene->CreateTexture("diffuse2B.png");
	std::shared_ptr<Texture> spec2B = BasicScene->CreateTexture("specular2B.png");
	std::shared_ptr<DirLight> luzdir = std::make_shared<DirLight>(vec3(0.5, 0.5, 0.5), vec3(0, 51.0f, -59.0f), vec3(0, -1, 0), 1.0f);
	BasicScene->addLightC(luzdir);
	for (int i = 0; i < 3; i++)
	{
		std::shared_ptr<Entity> go = BasicScene->CreateEntity();
		string name_mounter = "2B_" + std::to_string(i);
		go->set_name(name_mounter);
		//go->addComponent<ScriptComponent>();
		go->addComponent<RenderComponent>();
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMesh(cube_);
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMaterial();
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(tex2b);
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(spec2B);
		go->position(vec3(-8 + (8 * i), 1, 0));
		go->scale(vec3(0.1f));
	}
	auto go_plane = BasicScene->CreateEntity();
	go_plane->addComponent<RenderComponent>();
	go_plane->addComponent<ScriptComponent>();
	go_plane->getComponent<ScriptComponent>(kScriptComponent)->set_file("script.chai");
	go_plane->getComponent<RenderComponent>(kRenderComponent)->addMesh<Plane>();
	go_plane->getComponent<RenderComponent>(kRenderComponent)->addMaterial();
	go_plane->getComponent<RenderComponent>(kRenderComponent)->material_component()->addTexture(tex);
	go_plane->getComponent<RenderComponent>(kRenderComponent)->material_component()->addTexture(texspec);
	go_plane->scale(25, 1, 25);

}
void example_cubes(std::shared_ptr<Scene> BasicScene) {
	MTR_SCOPE("MAIN", "exampleCube")
	std::shared_ptr<Camera> cam = BasicScene->CreateCamera(vec3(0, 10, 10), vec2(10.0f, 100.0f), vec3(0), Camera::kPerspective);
	//std::shared_ptr<Camera> cam2 = BasicScene->CreateCamera(vec3(0, 51.0f, 59.0f), vec2(0.1f, 200.0f), vec3(0), Camera::kOrtho);
	std::shared_ptr<Texture> tex = BasicScene->CreateTexture("woodDiff.png");
	std::shared_ptr<Texture> texspec = BasicScene->CreateTexture("woodSpec.png");
	
	/*std::shared_ptr<DirLight> luzdir = std::make_shared<DirLight>(vec3(0.5, 0.5, 0.5), vec3(0, 51.0f, -59.0f), vec3(0, -1, 0), 1.0f);
	BasicScene->addLightC(luzdir);
	std::shared_ptr<DirLight> luzdir2 = std::make_shared<DirLight>(vec3(0.5, 0.5, 0.5), vec3(0, 51.0f, 59.0f), vec3(0, -1, 0), 1.0f);
	BasicScene->addLightC(luzdir2);*/
	for (int i = 0; i < 3; i++)
	{
		std::shared_ptr<Entity> go = BasicScene->CreateEntity();
		string name_mounter = "2B_" + std::to_string(i);
		go->set_name(name_mounter);
		//go->addComponent<ScriptComponent>();
		go->addComponent<RenderComponent>();
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMesh<Cube>();
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->addMaterial();
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(tex);
		go->getComponent<RenderComponent>(ComponentType::kRenderComponent)->material_component()->addTexture(texspec);
		go->addComponent<SineTransform>();
		go->getComponent<SineTransform>(ComponentType::KSineTransform)->setTypeAxix(SineAxis(i));
		go->position(vec3(-8 + (8 * i), 1, 0));
	}
	BasicScene->getEntityManager()->getList()[0]->addChild(BasicScene->getEntityManager()->getList()[1]);
	BasicScene->getEntityManager()->getList()[1]->addChild(BasicScene->getEntityManager()->getList()[2]);
	BasicScene->initShadowExam();
	/*auto go_plane = BasicScene->CreateEntity();
	go_plane->addComponent<RenderComponent>();
	go_plane->getComponent<RenderComponent>(kRenderComponent)->addMesh<Plane>();
	go_plane->getComponent<RenderComponent>(kRenderComponent)->addMaterial();
	go_plane->getComponent<RenderComponent>(kRenderComponent)->material_component()->addTexture(tex);
	go_plane->getComponent<RenderComponent>(kRenderComponent)->material_component()->addTexture(texspec);
	go_plane->scale(25, 1, 25);*/
	
}