#ifndef _H_DIR_LIGHT_
#define _H_DIR_LIGHT_

#include "light.h"
#include "Core/types.h"
namespace Awesome {
	class DirLight :public Light
	{
	public:
		DirLight(vec3 lightColor, vec3 lightPosition, vec3 lightDirection, float intensity);
		~DirLight();
		void Init();
		void setUniformInfo();
		void update(int8 id);
		float getIntesity()const;
		void setIntesity(float);
		uniform_faster_map get_UniformsInfo()const;
		
	private:
		//friend class cereal::access;
		uniform_faster_map uniform_info;
		float intensity_;
	};
}

#endif // !_H_DIR_LIGHT_
