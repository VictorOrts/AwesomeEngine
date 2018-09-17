#ifndef _H_USE_PROGRAM_COMMAND_
#define _H_USE_PROGRAM_COMMAND_

#include "command.h"
#include "Core/types.h"
#include "Core/OpenGL/aopengl.h"

namespace Awesome {

	class UseProgramCommand : public Command {
	public:
		UseProgramCommand(uint32 pro);
		~UseProgramCommand();
		void execute();
	private:
		uint32 program_;
	};

	UseProgramCommand::UseProgramCommand(uint32 pro)
	{
		program_ = pro;
	}

	UseProgramCommand::~UseProgramCommand()
	{
	
	}


	void UseProgramCommand::execute() {
		AOpenGL::UseProgram(program_);
	}
}
#endif // !_H_USE_PROGRAM_CIOMMAND_
