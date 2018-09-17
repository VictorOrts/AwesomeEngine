#include "Core/Commands/clear_command.h"
#include "Core/OpenGL/aopengl.h"
using namespace Awesome;
ClearCommand::ClearCommand(float r, float g, float b, float a)
{
	color = vec4(r, g, b, a);
}

ClearCommand::~ClearCommand()
{
}

void ClearCommand::execute() {
	//printf("pinta esto");
	AOpenGL::Clear(color);
}