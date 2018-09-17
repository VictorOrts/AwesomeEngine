#include "Core\input.h"
#include "deps\GLFW\glfw3.h"
#include "Core\engine.h"
#include "Core\Objects\utils.h"
using namespace Awesome;
//GLFWwindow * win;
Input::Input()
{
	initialized_ = 0;
	//win = (GLFWwindow*)Engine::engine_instance()->window()->window_ptr();
}

Input::~Input()
{
}

bool Input::keyPressed(Input::KeyBoard_Input key) {
	int16 state = glfwGetKey((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), key);
	if (state == GLFW_PRESS) {
		Engine::engine_instance()->add_log(string("INPUT " + std::to_string(key)), kLogColors::kColor_Green);
		return true;
	}
	else {
		return false;
	}
}

bool Input::MousePressed(Input::Mouse_Input button) {
	int16 state = glfwGetMouseButton((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), button);
	if (state == GLFW_PRESS) {
		Engine::engine_instance()->add_log(string("MOUSE " + std::to_string(button)), kLogColors::kColor_Green);
		return true;
	}
	else {
		return false;
	}
}

void Input::mouseCursor(double *xpos, double *ypos) {
	glfwGetCursorPos((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), xpos, ypos);
}

void Input::setmouseCursor(double x ,double y) {
	glfwSetCursorPos((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), x, y);
}

float Input::getMouseX() {
	double x, y;
	glfwGetCursorPos((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), &x, &y);
	return (float)x;
}
float Input::getMouseY(){
	double x, y;
	glfwGetCursorPos((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), &x, &y);
	return (float)y;
}

void Input::setmouseVisibility(bool mode) {
	if (mode == true) {
		glfwSetInputMode((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	}
	else {

		glfwSetInputMode((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	}
}