#include "Core/Task/gameobjectstask.h"

using namespace Awesome;
UpdateGoTask::UpdateGoTask(std::shared_ptr<Entity> rgo)
{
	tmpGO_ = rgo;
}

UpdateGoTask::~UpdateGoTask()
{
}

void UpdateGoTask::execute() {
	//printf("\nTASK::UPDATE_GO_TASK %s",tmpGO_->name().c_str());
	tmpGO_->visitornode(tmpGO_->shared_from_this());
}