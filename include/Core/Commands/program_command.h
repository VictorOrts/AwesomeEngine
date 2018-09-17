#ifndef _H_PROGRAM_COMMAND_
#define _H_PROGRAM_COMMAND_

#include "command.h"
#include "../types.h"
#include "Core/material.h"
namespace Awesome {
	class ProgramCommand :public Command
	{
	public:
		ProgramCommand(uint32 program,Material* r_mat, mat4 mat,mat4 camera);
		~ProgramCommand();
		void execute();
	private:
		uint32 program_;
		Material* este;
		mat4 model_;
		mat4 proyection_view_;
	};
}

#endif // !_H_PROGRAM_COMMAND_
