#ifndef _H_CAMERA_
#define _H_CAMERA_
#include "Core/types.h"
namespace Awesome {
	class Camera
	{
	public:
		enum TypeCamera {
			kPerspective,
			kOrtho
		};
		enum CameraMove
		{
			kForward = 0,
			kBack,
		};
		Camera(vec3 position,vec2 nearfar,vec3 look, TypeCamera tc = kPerspective);
		~Camera();
		void set_camera(vec3 position, float near = 0.1f, float far_ = 500.0f);
		mat4 view_matrix();
		mat4 projection_matrix();
		mat4 view_projection_matrix();
		void move_camera(CameraMove cmove, float time_x_spedd);

		vec3 getPosition() ;
		void setPosition( vec3 position);

		vec2 getRotation() ;
		void setRotation(vec2 rotation);
		void resetRotation();

		vec3 getDirection() ;

		void setDirection( vec3 direction);

		void lookAt(vec3 point);

		void setNearFar(float near, float far);
		float getNear() const;
		float getFar() const;
		TypeCamera getTypecamera() const;
	private:
		//Private fuction auto update if something changes
		void updateView();
		void updateDirection();
		void updateRotation();
	
		TypeCamera camera_type_;
		mat4 view_;
		mat4 projection_;
		vec3 position_;
		vec2 rotation_;
		vec3 direction_;
		float field_of_view_;
		vec2 near_far_;
		const float verticalAngle = 89.0f;
		const float horizontalAngle = 360;
	};
}

#endif // !_H_CAMERA_
