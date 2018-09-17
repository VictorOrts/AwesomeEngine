#include "Core/Task/displaytask.h"
#include "Core/Components/rendercomponent.h"
#include "Core/Commands/framebuffer_command.h"
#include "Core/engine.h"
using namespace Awesome;

DisplayTask::DisplayTask(Scene* este)
{
	scene_ = este;
}

DisplayTask::~DisplayTask() {
}

void DisplayTask::renderScene(mat4 cameratoRender, uint32 program) {
	Engine * tmpEngine = Engine::engine_instance();

	//AQUI AÑADIRIA UN COMANDO DE START ! ASI LA LISTA SABRA CUANDO ABRIRSE Y  ESPERAR A UN END
	 // o crear aqui una lista y añadirla ! 
	if (program != tmpEngine->get_render()->program(RenderMaterial::kDepth)) {
		std::shared_ptr<ClearCommand> clear = std::make_shared<ClearCommand>(0.0f, 0.0f, 0.0f, 1.0f);
		tmpEngine->get_render()->get_display()->addCommand(clear);
	}
	for (size_t i = 0; i < scene_->getEntityManager()->getList().size(); i++)
	{
		if (scene_->getEntityManager()->getList()[i]->hasComponent(kRenderComponent) == 1) {
			scene_->getEntityManager()->getList()[i]->getComponent<RenderComponent>(kRenderComponent)->render(cameratoRender, program);
			for (uint32 o = 0; o < scene_->get_lights().size(); o++) {
				scene_->get_lights()[o]->update(0);
			}
		}


	}
	//AQUI AÑADIRIA UN COMANDO DE END ! ASI LA LISTA SABRA CUANDO CERRARSE ! Y PASAR A OTRA NUEVA
}
void DisplayTask::execute() {
	//Compruebo que hayan camaras sino rompera mas adelante !
	if (scene_->currentCamera() != nullptr) {
		forwardRendering();
	}
}

void DisplayTask::forwardRendering() {
	Engine * tmpEngine = Engine::engine_instance();

	for (uint8 i = 0; i < scene_->get_lights().size(); i++) {
		if (scene_->get_lights()[i]->get_cast_shadows() == true) {
			//printf("\nCastean sombras");
			std::shared_ptr<BeginFrameBufferCommand> startShadow = std::make_shared<BeginFrameBufferCommand>(scene_->get_lights()[i]->get_Shadow().get());
			tmpEngine->get_render()->get_display()->addCommand(startShadow);

			renderScene(scene_->get_lights()[i]->getViewMatrixProjection(), tmpEngine->get_render()->program(RenderMaterial::kDepth));

			std::shared_ptr<EndFrameBufferCommand> endShadow = std::make_shared<EndFrameBufferCommand>(scene_->get_lights()[i]->get_Shadow().get());
			tmpEngine->get_render()->get_display()->addCommand(endShadow);
		}
	}
	//DEPTH EXAM
	if (scene_->shadowExam != nullptr) {
		std::shared_ptr<BeginFrameBufferCommand> startShadow = std::make_shared<BeginFrameBufferCommand>(scene_->shadowExam.get());
		tmpEngine->get_render()->get_display()->addCommand(startShadow);
		renderScene(scene_->camera()[0]->view_projection_matrix(), tmpEngine->get_render()->program(RenderMaterial::kDepth));
		std::shared_ptr<EndFrameBufferCommand> endShadow = std::make_shared<EndFrameBufferCommand>(scene_->shadowExam.get());
		tmpEngine->get_render()->get_display()->addCommand(endShadow);
	}

	/*
	std::shared_ptr<BeginFrameBufferCommand> startShadow = std::make_shared<BeginFrameBufferCommand>(scene_->get_shadow().get());
	tmpEngine->get_render()->get_display()->addCommand(startShadow);
	renderScene(scene_->camera()[1]->view_projection_matrix(), tmpEngine->get_render()->program(RenderMaterial::kDepth));
	std::shared_ptr<EndFrameBufferCommand> endShadow = std::make_shared<EndFrameBufferCommand>(scene_->get_shadow().get());
	tmpEngine->get_render()->get_display()->addCommand(endShadow);
	*/
	std::shared_ptr<BeginFrameBufferCommand> start = std::make_shared<BeginFrameBufferCommand>(scene_->get_rtt().get());
	tmpEngine->get_render()->get_display()->addCommand(start);

	renderScene(scene_->currentCamera()->view_projection_matrix(), tmpEngine->get_render()->program());

	std::shared_ptr<EndFrameBufferCommand> end = std::make_shared<EndFrameBufferCommand>(scene_->get_rtt().get());
	tmpEngine->get_render()->get_display()->addCommand(end);


}
