#ifndef _H_TIME_
#define _H_TIME_
#include "types.h"
#include <chrono>
namespace Awesome {
	class Time {
	public:
		Time();
		~Time();
		double last_time();
		void  last_time_add(double add);
		double current_time();
		int16 frames();
		void reset_frames();
		void set_frames();
		void show_ms();
		bool next_frame();
		float time_elapsed();
		//////////

	private:
		int16 frames_;
		double last_time_;
		double current_time_;
	};
}
#endif // !_H_TIME_

