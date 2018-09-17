
#ifndef _H_SPOTLIGHT_
#define _H_SPOTLIGHT_

#include "light.h"
#include "Core/types.h"

namespace Awesome {
	class SpotLight : public Light
	{
	public:
		SpotLight(glm::vec3 lightColor, glm::vec3 lightPosition, glm::vec3 lightDirection, float cutOff, float outerCutOff, float constant, float linear, float quadratic);
		~SpotLight();
		float getCutOff();
		float getOuterCutOff();
		float getConstant();
		float getLinear();
		float getQuadratic();
		void setUniformInfo();
		void update(int8 id);
		uniform_faster_map get_UniformsInfo();
	private:
		uniform_faster_map uniform_info;
		float cutOff_;
		float outerCutOff_;
		float constant_;
		float linear_;
		float quadratic_;
	};


}
#endif // !_H_SPOTLIGHT_
