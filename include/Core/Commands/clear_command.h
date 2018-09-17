#ifndef _H_CLEAR_COMMAND_
#define _H_CLEAR_COMMAND_

#include "command.h"
#include <stdio.h>
#include "../types.h"

namespace Awesome {

	class ClearCommand :public Command
	{
	public:
		ClearCommand(float r, float g, float b, float a);
		~ClearCommand();
		void execute();
	private:
		vec4 color;
	};

}
#endif // !_H_CLEAR_COMMAND_
