#ifndef _H_TASK_
#define _H_TASK_


namespace Awesome {
	class Task {
	public:
		Task();
		~Task();
		virtual void execute() = 0;
		bool isFinished();
		void finished();

	private:
		bool finished_;
	};
}

#endif // !_H_TASK_
