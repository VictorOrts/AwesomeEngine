#ifndef _H_SPOTLIGHT_COMMAND_
#define _H_POTLIGHT_COMMAND_

#include "command.h"
#include "Core/Light/spotlight.h"
namespace Awesome {
	class SpotLightCommand : public Command
	{
	public:
		SpotLightCommand(SpotLight * spot);
		~SpotLightCommand();
		void execute();
	private:
		SpotLight * spl;
		uint32 program_;
	};
}
#endif
