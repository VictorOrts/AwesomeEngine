#ifndef _H_TASK_MANAGER_
#define _H_TASK_MANAGER_

#include "types.h"
#include "Task/task.h"

#include <atomic>
#include <thread>
#include <mutex>
#include <array>
#include <list>
#include <functional>
#include <condition_variable>


namespace Awesome {
	class TaskManager
	{
	public:

		TaskManager();
		~TaskManager();
		typedef std::shared_ptr<Task> TTask;
		//////////////////
		void updateFor();
		/**
		 * @brief      { Execute the function of the task called execute ,rest one job }
		 */
		void executeTask();

		/**
		 * @brief      { Get the next task in the queue }
		 *
		 * @return     { return the next task }
		 */
		TTask next_task();

		/**
		 * @brief      Size of the thread or number of threads
		 *
		 * @return     return the size of the threads
		 */
		size_t Size() const;

		// @param[in]  job   Add a task to the queue
		//
		void AddTask(TTask job);

		/**
		 * @brief      { function_description }
		 *
		 * @param[in]  WaitForAll  The wait for all called the engine is closing need true to do  wait
		 */
		void JoinAll(bool WaitForAll);

		/**
		 * @brief      Espera a todos
		 */
		void WaitAll();

		void numberJobsDone();
	private:
		//Save the value of std::thread::hardware_concurrency();
		size_t max_thread;
		//Vector threads
		std::thread updateAll;
		std::vector<std::thread> threads;
		//list of task to do
		std::list<TTask>		list_task_;
		//Atomic mejor que normal variables
		std::atomic_int			jobs_done;
		std::atomic_int			jobs_left;
		std::atomic_bool        jobs_to_do;
		std::atomic_bool        finished;
		//2 condition variables better than one for easy control instead using one
		std::condition_variable job_available_var;
		std::condition_variable wait_var;
		// mutex uno para la cola y otra wait
		std::mutex              wait_mutex;
		std::mutex              queue_mutex;

	};
}

#endif // !_H_TASK_MANAGER_

/*
wait
mutexscope lock(mutex)
comprobacion hayan elementos
cojo el elemneto
post

Render		 ......			Update
Semaforo
while		 post			while
wait

SemPostCommand
*/