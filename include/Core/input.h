#ifndef _H_INPUT_
#define _H_INPUT_

#include "types.h"

namespace Awesome {
	class Input
	{
	public:
		enum KeyBoard_Input {
			kSpecial_Space=32,
			kInput_0 = 48,
			kInput_1,
			kInput_2,
			kInput_3,
			kInput_4,
			kInput_5,
			kInput_6,
			kInput_7,
			kInput_8,
			kInput_9,
			kInput_A = 65,
			kInput_B,
			kInput_C,
			kInput_D,
			kInput_E,
			kInput_F,
			kInput_G,
			kInput_H,
			kInput_I,
			kInput_J,
			kInput_K,
			kInput_L,
			kInput_M,
			kInput_N,
			kInput_O,
			kInput_P,
			kInput_Q,
			kInput_R,
			kInput_S,
			kInput_T,
			kInput_U,
			kInput_V,
			kInput_W,
			kInput_X,
			kInput_Y,
			kInput_Z,
			KSpecial_Escape = 256,
		};
		enum Mouse_Input {
			kInput_Mouse_Left = 0, //Izquierdo
			kInput_Mouse_Right,	//Derecho
			kInput_Mouse_Wheel,	//Wheel press
			kInput_Mouse_SideDown, //lateral down
			kInput_Mouse_SideUp,	//lateral up
			kInput_Mouse_6, // dont know
			kInput_Mouse_7	// dont know
		};
		Input();
		~Input();
		bool keyPressed(KeyBoard_Input key);
		bool MousePressed(Mouse_Input button);
		void mouseCursor(double *xpos, double *ypos);
		void setmouseCursor(double x, double y);
		void setmouseVisibility(bool mode);
		float getMouseX();
		float getMouseY();
	private:
		int16 initialized_;
	};
}
#endif // !_H_INPUT_
