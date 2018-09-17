#ifndef _H_LIGHT_COMMAND_
#define _H_LIGHT_COMMAND_

#include "command.h"
#include "Core/types.h"
#include "Core/Light/light.h"

namespace Awesome {
	class DirLightCommand : public Command
	{
	public:
		DirLightCommand(Light *a);
		~DirLightCommand();
		void execute();
	private:
		uint32 program_;
		Light * tmp;
	};


}

#endif