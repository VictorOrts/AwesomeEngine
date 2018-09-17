#ifndef _H_DRAW_COMMAND_
#define _H_DRAW_COMMAND_

#include "command.h"
#include "../types.h"

namespace Awesome {
	class DrawCommand :public Command
	{
	public:
		DrawCommand(uint32, uint32, uint32);
		~DrawCommand();
		void execute();
	private:
		uint32 evo_;
		uint32 vao_;
		uint32 indices_;
	};
}
#endif // !_H_DRAW_COMMAND_
