#ifndef _H_SINE_TRANSFORM_COMPONENT_
#define _H_SINE_TRANSFORM_COMPONENT_

#include "Core/types.h"
#include "Core/component.h"



namespace Awesome {
	enum SineAxis {
		Sine_X,
		Sine_y,
		Sine_Z
	};

	class SineTransform :public Component {
	public:
		SineTransform();
		~SineTransform();
		void setTypeAxix(SineAxis a);
		void update();
		void render(mat4 transform, uint32 program);
		void func() {
			printf("SINETRANSFORM::FUNC");
		}
	private:
		SineAxis valueSine;
	};
}

#endif

