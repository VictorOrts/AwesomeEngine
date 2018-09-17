#ifndef _H_POINTLIGHT_
#define _H_POINTLIGHT_


#include "light.h"
#include "Core/types.h"
namespace Awesome {
	class PointLight : public Light
	{
	public:
		PointLight(vec3 lightColor, vec3 lightPosition, float constant, float linear, float quadratic);
		~PointLight();
		void update(int8 id);
		float getConstant();
		float getLinear();
		float getQuadratic();
		void setUniformInfo();
		uniform_map get_UniformsInfo();
	private:
		uniform_map uniform_info;
		float constant_;
		float linear_;
		float quadratic_;
	};
}

#endif // !_H_POINTLIGT_
/*
_uniformManager.updateUniformData("pointLights[" + std::to_string(pointCNT) + "].quadratic",
						pL->getQuadratic());*/