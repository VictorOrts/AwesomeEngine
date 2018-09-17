#ifndef _H_DISPLAY_TASK_
#define _H_DISPLAY_TASK_

#include "task.h"
#include "Core/types.h"
#include "Core/scene.h"
//#include "Core/Objects/texture.h"
#include "Core/Commands/command.h"
//#include "Core/Task/displaytask.h"
#include <vector>
namespace Awesome {
	class DisplayTask : public  Task {
	public:
		DisplayTask(Scene* este);
		~DisplayTask();
		typedef std::shared_ptr<Command> TCCommand;
		void renderScene(mat4 cameratoRender, uint32 program);
		void execute();
		void forwardRendering();
	private:
		Scene* scene_;
		std::vector<TCCommand> display_;
	};
}

#endif // !_H_DISPLAY_TASK_
