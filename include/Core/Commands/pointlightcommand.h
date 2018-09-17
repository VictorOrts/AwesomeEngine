#ifndef _H_POINTLIGHT_COMMAND_
#define _H_POINTLIGHT_COMMAND_

#include "command.h"
#include "Core/Light/pointlight.h"
namespace Awesome {
	class PointLightCommand : public Command
	{
	public:
		PointLightCommand(PointLight * rlight);
		~PointLightCommand ();
		void execute();
	private:
		uint32 program_;
		PointLight * pl;
	};

}


#endif // !_H_POINTLIGHT_COMMAND_

