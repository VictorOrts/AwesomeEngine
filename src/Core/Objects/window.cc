#include "../include/Core/Objects/window.h"
#include <stdio.h>
#include "../include/deps/GL/glew.h"
#include "../include/deps/GLFW/glfw3.h"
#include "Core\OpenGL\aopengl.h"
#include <iostream>
using namespace Awesome;

struct Window::WindowData {
	GLFWwindow *screen;
	int16 width;
	int16 height;
	WindowData() {
		screen = nullptr;
		width = 0; height = 0;
	}
};

Window::Window() {
	data_ptr_ = new WindowData();
}
Window::~Window() {
	data_ptr_->screen = nullptr;
	delete data_ptr_;
}

int16 Window::window_height() {
	return data_ptr_->height;
}

int16 Window::window_width() {
	return data_ptr_->width;
}

void* Window::window_ptr() {
	return data_ptr_->screen;
}

Window::Window(const Window&other) {
	data_ptr_ = other.data_ptr_;
}

int main(int32 argv, char**argc) {
	Window::main(argv, argc);
	return 0;
}

bool Window::init(char_ptr name, WindowResolution res) {
	if (!glfwInit()) {
		printf("GLFW init error.\n");
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	switch (res)
	{
	case kResolution_1920x1080: data_ptr_->width = 1920; data_ptr_->height = 1080;
		break;
	case kResolution_1600x900: data_ptr_->width = 1600; data_ptr_->height = 900;
		break;
	case kResolution_1440x900: data_ptr_->width = 1440; data_ptr_->height = 900;
		break;
	case kResolution_1366x768: data_ptr_->width = 1366; data_ptr_->height = 768;
		break;
	case kResolution_1280x720: data_ptr_->width = 1280; data_ptr_->height = 720;
		break;
	case kResolution_800x600: data_ptr_->width = 800; data_ptr_->height = 600;
		break;
	default: data_ptr_->width = 1280; data_ptr_->height = 720;
		break;
	}
	data_ptr_->screen = glfwCreateWindow(data_ptr_->width, data_ptr_->height, name, nullptr, data_ptr_->screen);
	if (data_ptr_->screen == nullptr) {
		printf("Error: Window not created.\n");
		return false;
	}
	glfwMakeContextCurrent(data_ptr_->screen);
	glewExperimental = true;

	if (glewInit() != GLEW_OK) {
		printf("Error to initialize GLEW.\n");
		return false;
	}
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
	std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	
	glViewport(0, 0, data_ptr_->width, data_ptr_->height);
	glEnable(GL_CULL_FACE);
	glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//For nvidia
	//glfwSwapInterval(0);
	return true;
}

bool Window::processEvents() {
	glfwPollEvents();
	return true;
}

void Window::clear() {
	AOpenGL::Clear(vec4(1, 0, 0, 1));
}
void Window::swap() {
	glfwSwapBuffers(data_ptr_->screen);
}

bool Window::shouldCloseWindow() {
	if (!glfwWindowShouldClose(data_ptr_->screen)) {
		return false;
	}
	else {
		return true;
	}
}

void Window::finish() {
	glfwTerminate();
}