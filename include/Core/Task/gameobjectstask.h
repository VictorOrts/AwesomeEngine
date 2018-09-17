#ifndef _H_OBJ_TASK_
#define _H_OBJ_TASK_
#include "Core/Task/task.h"
#include "Core/Objects/entity.h"
namespace Awesome {
	class UpdateGoTask :public Task {
	public:
		UpdateGoTask(std::shared_ptr<Entity>);
		~UpdateGoTask();
		void execute();
	private:
		std::shared_ptr<Entity> tmpGO_;
	};
}

#endif // !_H_OBJ_TASK_
