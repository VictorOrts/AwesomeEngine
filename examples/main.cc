#include "Core/awesomeengine.h"
#include "external/minitrace.h"


using namespace Awesome;
const float speed = 1.0f;

#include "2Bexample.cc"
int Window::main(int argc, char**argv) {
	mtr_init("mt_trace.json");
	MTR_META_PROCESS_NAME("Multithreaded Test");
	MTR_META_THREAD_NAME("Main Thread");
	Engine * myengine = Engine::engine_instance();
	myengine->initEngine("Demo", WindowResolution::kResolution_800x600);
	std::shared_ptr<Scene> BasicScene = std::make_shared<Scene>();
	myengine->setScene(BasicScene);

	//Function already maded for example scenes
	example_2B(BasicScene);
	//example_cubes(BasicScene);
	myengine->get_time()->last_time();
	double posx, posy;
	vec2 center = vec2(400, 300);
	vec2 tempSize;
	Utils::Log("PruebaColor", kLogColors::kColor_Green);
	tempSize.x = myengine->window()->window_width()/2;
	tempSize.y = myengine->window()->window_height()/2;
	
	while (myengine->window()->shouldCloseWindow() || myengine->get_input()->keyPressed(Input::KSpecial_Escape) != 1) {
		myengine->get_time()->current_time();
		myengine->get_time()->set_frames();
		myengine->window()->processEvents();
		
		if (BasicScene->currentCamera() != nullptr) {
			if (myengine->get_input()->MousePressed(Input::kInput_Mouse_Right)) {
				float sens = 0.15f;
				myengine->get_input()->setmouseVisibility(false);
				myengine->get_input()->mouseCursor(&posx, &posy);
				vec2 diferencia = center - vec2(posx, posy);
				if (diferencia.x != 0 || diferencia.y != 0) {
					vec2 new_rot = vec2(myengine->currentScene()->currentCamera()->getRotation().x + diferencia.y * sens, myengine->currentScene()->currentCamera()->getRotation().y + diferencia.x * sens);
					myengine->currentScene()->currentCamera()->setRotation(new_rot);
					myengine->get_input()->setmouseCursor(tempSize.x, tempSize.y);
				}
			}
			else {
				myengine->get_input()->setmouseVisibility(true);
			}



			if (myengine->get_input()->keyPressed(Input::kInput_W)) {
				myengine->currentScene()->currentCamera()->move_camera(Camera::kForward, myengine->get_time()->time_elapsed()* speed);
			}
			if (myengine->get_input()->keyPressed(Input::kInput_S)) {
				myengine->currentScene()->currentCamera()->move_camera(Camera::kBack, myengine->get_time()->time_elapsed()* speed);
			}

			if (myengine->get_input()->keyPressed(Input::kInput_V)) {
				myengine->get_render()->SetRenderMaterial(RenderMaterial::kDepth);
			}
		}
		myengine->run();
		myengine->window()->swap();
		if (myengine->get_time()->next_frame()) {
			
		}

	}

	mtr_flush();
	mtr_shutdown();
	myengine->closeEngine();
	return 0;
}


