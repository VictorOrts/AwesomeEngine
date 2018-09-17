#ifndef _H_UTILS_
#define _H_UTILS_


#include "Core/types.h"

namespace Awesome {

	enum kLogColors
	{
		kColor_DarkBlue=9,
		kColor_Green,
		kColor_Watermarine,
		kColor_Red,
		kColor_Yellow=14,
		kColor_Default,//White
	};
	enum kLogOutput {
		kWindow_OGL,
		kWindow_CMD
	};


	namespace Utils {
		//Function for read files
		string ReadFromFile(string file);
		//Function to display logs
		void Log(string a, kLogColors color,kLogOutput logOut=kWindow_CMD);
		//Function to  take the hour
		string TimeHourMinutesSeconds();
		//Function for conver floats to ASD
		string limit_float_precision(float val);
		string limit_float_precision(vec3 val);
		string limit_float_precision(vec2 val);

	}
}
#endif // !_H_UTILS_
