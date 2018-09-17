#include "Core/Commands/draw_command.h"
#include "Core/OpenGL/aopengl.h"
using namespace Awesome;
DrawCommand::DrawCommand(uint32 ind, uint32 vao, uint32 evo)
{
	indices_ = ind;
	vao_ = vao;
	evo_ = evo;
}

DrawCommand::~DrawCommand()
{
}

void DrawCommand::execute() {
	//printf("pinta esto");
	AOpenGL::DrawElements(0, indices_, vao_, evo_);
}