#include "Core\taskmanager.h"
#include "Core\engine.h"
#include "Core\Task\texturetask.h"
#include "Core\Task\objtask.h"
using namespace Awesome;

TaskManager::TaskManager()
{
	jobs_done = 0;
	jobs_left = 0;
	jobs_to_do= false;
	finished = false ;
	max_thread = std::thread::hardware_concurrency()/2;
	//Init the vector of threads
	Engine::engine_instance()->add_log(std::string("NumCores " + std::to_string(max_thread)), kLogColors::kColor_Default);
	for (uint16 i = 0; i < max_thread; i++) {
		//Funcion a ejecutar dentro
		threads.push_back(std::move(std::thread([this, i] { this->executeTask(); })));
	}
	//updateAll = std::thread(&TaskManager::updateFor, this);
	//updateAll = std::thread(&TaskManager::updateFor);
	//updateFor();
}

TaskManager::~TaskManager() {
	//updateAll.join();
	threads.clear();
	
}
void TaskManager::updateFor() {
	/*std::shared_ptr<Scene> curr =  Engine::engine_instance()->currentScene();
	while (1) {
		curr->update();
		//printf("Cosas");

	}*/
}

void TaskManager::executeTask() {
	while (!jobs_to_do) {
		next_task()->execute();
		--jobs_left;
		wait_var.notify_one();
	}
}

TaskManager::TTask TaskManager::next_task() {
	TTask res;
	std::unique_lock<std::mutex> job_lock(queue_mutex);
	job_available_var.wait(job_lock, [this]()->bool {return list_task_.size() || jobs_to_do; });
	if (!jobs_to_do) {
		res = list_task_.front();
		list_task_.pop_front();
	}
	else {
		res = nullptr;
		++jobs_left;
	}
	return res;
}

size_t TaskManager::Size() const {
	return max_thread;
}


void TaskManager::AddTask(TTask job) {
	std::lock_guard<std::mutex> guard(queue_mutex);
	list_task_.emplace_back(job);
	++jobs_left;
	++jobs_done;
	job_available_var.notify_one();
}

void TaskManager::JoinAll(bool WaitForAll) {
	if (!finished) {
		if (WaitForAll) {
			WaitAll();
		}
		jobs_to_do = true;
		job_available_var.notify_all();
		//Auto para no poner el tipo ! sino .. std::vector y recorrer<std::thread>
		// mas rapido y comodo el auto
		for (auto &x : threads) {
			if (x.joinable()) {
				x.join();
			}
		}
		finished = true;
	}
}

void TaskManager::WaitAll() {
	if (jobs_left > 0) {
		std::unique_lock<std::mutex> lk(wait_mutex);
		//trabajos que le quedan en el wait
		wait_var.wait(lk, [this] { return this->jobs_left == 0; });
		lk.unlock();
	}
}

void TaskManager::numberJobsDone() {
	Utils::Log(string("Number jobs done " + std::to_string(jobs_done)), kLogColors::kColor_Default, kLogOutput::kWindow_CMD);
}