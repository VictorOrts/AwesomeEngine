/*
*	Light class
*	Basic class for PointLight DirLight and SpotLight
*	by Victor Orts
*/

#ifndef _H_LIGHT_
#define _H_LIGHT_
#include "Core/types.h"
#include "Core/Objects/shadowtexture.h"
#include "Core/Objects/camera.h"
namespace Awesome {

	class Light
	{
	public:
		Light();
		virtual ~Light();
		LightType type_light()const;
		void set_type_light(LightType a) {
			typeL = a;
		}
		virtual void setUniformInfo()=0;
		virtual void update(int8 id) = 0;
		virtual void Init() = 0;
		void set_position(vec3 a);
		vec3 position();
		void set_direction(vec3 a);
		vec3 direction();
		vec3 color();
		void set_color(vec3);
		bool get_cast_shadows()const ;
		void set_cast_shadows(bool active);
		std::shared_ptr<ShadowTexture> get_Shadow()const;
		mat4 getViewMatrixProjection();
		void set_numberID_light(int8 number);
		int8 get_numberID();
		string get_name();
	protected :
		std::shared_ptr<Camera> lightCamera_;
		bool cast_shadow_;
		int8 numberID_;
		string name_;
	private:
		
		//Light Basics
		std::shared_ptr<ShadowTexture> shadow_;
		
		LightType typeL;
		vec3 position_;
		vec3 direction_;
		vec3 light_color_;
	};
}
#endif // !H_LIGHT_
