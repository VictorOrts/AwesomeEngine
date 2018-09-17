#include "Core\time.h"
#include "deps\GLFW\glfw3.h"
#include "Core\Objects\utils.h"
using namespace Awesome;

Time::Time() {
	frames_ = 0;
	//last_time_ = glfwGetTime();
}

Time::~Time() {
}

double Time::current_time() {
	current_time_ = glfwGetTime();
	return  current_time_;
}

double Time::last_time() {
	last_time_ = glfwGetTime();
	return last_time_;
}

void Time::last_time_add(double add) {
	last_time_ += add;
}
int16 Time::frames() {
	return frames_;
}

void Time::reset_frames() {
	frames_ = 0;
}

void Time::set_frames() {
	frames_++;
}

void Time::show_ms() {
	string show = std::to_string(1000.0 / double(frames_))+ " ms/frame/"+std::to_string(frames_)+ "/fps";
	Utils::Log(show, kLogColors::kColor_Default,kLogOutput::kWindow_CMD);
}
float Time::time_elapsed() {
	return (float)(current_time_ - last_time_);
}
bool Time::next_frame() {
	if (current_time_ - last_time_ >= 1.0f) {
		show_ms();
		reset_frames();
		last_time_add(1.0);
		return true;
	}
	else {
		return false;
	}
}

