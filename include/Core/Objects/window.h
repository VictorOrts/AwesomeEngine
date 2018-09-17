#ifndef __WINDOW_H__
#define __WINDOW_H__
#include "../include/Core/types.h"
namespace Awesome {
	class Window {
	public:
		enum WindowResolution
		{
			kResolution_1920x1080 = 0,
			kResolution_1600x900,
			kResolution_1440x900,
			kResolution_1366x768,
			kResolution_1280x720,
			kResolution_800x600,
		};
		Window();
		~Window();
		Window(const Window&);
		static int main(int32 argc, char**argv);
		bool init(char_ptr name, WindowResolution res);
		bool processEvents();
		void swap();
		void finish();
		void clear();
		bool shouldCloseWindow();
		//Getters
		int16 window_height();
		int16 window_width();
		void* window_ptr();
	private:
		struct WindowData;
		WindowData *data_ptr_;
	};
}
#endif // !__H_WINDOW__
