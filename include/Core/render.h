/*
*	Class Render
*	Manage the rendering and execute the commands for draw.
*	Contain the program of the shader
*	by Victor Orts
*/

#ifndef _H_RENDER_
#define _H_RENDER_

#include <vector>
#include "types.h"
#include "displaylist.h"
#include "Core/Commands/clear_command.h"
#include "Core/Commands/program_command.h"
#include "Core/Objects/texture.h"
namespace Awesome {
	class Render {
	public:
		Render();
		~Render();
		void run();
		std::shared_ptr<Displaylist> get_display();
		uint32 program(RenderMaterial tipo= kNormal) {
			switch (tipo)
			{
			case kDiffuse:
				return program_;
				break;
			case kDepth:
				return program_depth;
				break;
			default:
				return program_;
				break;
			}
		}
		void SetRenderMaterial(RenderMaterial type) {
			rm_ = type;
		}
		void set_displaylist(std::shared_ptr<Displaylist>);
		void add_ComandtmpDL(std::shared_ptr<Command> tmp);
	private:
		bool finished_;
		RenderMaterial rm_;
		uint32 program_;
		uint32 program_depth;
		std::vector<std::shared_ptr<Displaylist>> vectorDisplayList_;
		std::shared_ptr<Displaylist> display_;
	};
}
#endif // !_H_RENDER_
