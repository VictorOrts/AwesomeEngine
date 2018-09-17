#include "Core/Task/task.h"

using namespace Awesome;

Task::Task()
{
	finished_ = false;
}

Task::~Task()
{
}

void Task::finished() {
	finished_ = true;
}

bool Task::isFinished() {
	return finished_;
}