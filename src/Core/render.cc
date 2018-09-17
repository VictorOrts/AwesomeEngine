#include "Core\render.h"
#include "Core\engine.h"
#include "Core\OpenGL\aopengl.h"
using namespace Awesome;
Render::Render() {
	display_ = std::make_shared<Displaylist>();
	program_ = AOpenGL::CreateShader("vertexShader.glsl", "fragmentShader.glsl");
	program_depth = AOpenGL::CreateShader("vertex.glsl", "fragment.glsl");
	finished_ = true;
}

Render::~Render() {
	AOpenGL::DeleteProgram(program_);
	AOpenGL::DeleteProgram(program_depth);
}

std::shared_ptr<Displaylist> Render::get_display(){
	return display_;
}



void Render::run() {
	display_->runDisplaylist();
	/*for (int i = 0; i < vectorDisplayList_.size(); i++) {
		vectorDisplayList_[i]->runDisplaylist();
	}*/
}

void Render::set_displaylist(std::shared_ptr<Displaylist> tmpList) {
	vectorDisplayList_.push_back(tmpList);
}

void Render::add_ComandtmpDL(std::shared_ptr<Command> tmpCommand) {
	
	if (tmpCommand == nullptr) {

	}
	else {
		display_->addCommand(tmpCommand);
	}
}