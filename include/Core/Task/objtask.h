#ifndef _H_OBJ_TASK_
#define _H_OBJ_TASK_
#include "Core/Task/task.h"
#include "Core/Geometry/obj.h"
namespace Awesome {
	class ObjTask :public Task {
	public:
		ObjTask();
		~ObjTask();
		void execute();
	private:
	};
}

#endif // !_H_OBJ_TASK_
