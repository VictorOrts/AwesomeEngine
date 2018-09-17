#ifndef _H_MYCEREALSAVES_
#define _H_MYCEREALSAVES_

#include "Core/types.h"
#include "Core/Light/dirlight.h"
#include "Core/Light/spotlight.h"
#include "Core/Light/pointlight.h"
#include "Core/Objects/camera.h"
#include "Core/Objects/entity.h"
//STRUCTS FOR SAVE


namespace Awesome {
	struct cerealEntity
	{
		vec3 position;
		vec3 scale;
		vec3 rotation;
		std::string name_;

		void SetEntity(Entity entity)
		{
			position = entity.getPosition();
			scale = entity.getScale();
			rotation = entity.getRotation();
			name_ = entity.name();
		}

	};
	struct cerealSpotLight
	{
		int typeL;
		vec3 position;
		vec3 direction;
		vec3 color;
		float cutoff;
		float outercutoff;
		float constant;
		float linear;
		float quadratic;

		void SetSpotlight(SpotLight& light)
		{
			position = light.position();
			direction = light.direction();
			color = light.color();
			typeL = light.type_light();
			cutoff = light.getCutOff();
			outercutoff = light.getOuterCutOff();
			constant = light.getConstant();
			linear = light.getLinear();
			quadratic = light.getQuadratic();

		}

	};
	struct cerealPointLight
	{
		int typeL;
		vec3 position;
		vec3 direction;
		vec3 color;
		float constant;
		float linear;
		float quadratic;

		void SetPoint(PointLight& light)
		{
			position = light.position();
			direction = light.direction();
			color = light.color();
			typeL = light.type_light();
			constant = light.getConstant();
			linear = light.getLinear();
			quadratic = light.getQuadratic();
		}
	};
	struct cerealDirectionalLight
	{
		int typeL;
		vec3 position;
		vec3 direction;
		vec3 color;
		float intesity;
		void SetDirectional(DirLight& light)
		{
			position = light.position();
			direction = light.direction();
			color = light.color();
			typeL = light.type_light();
			intesity = light.getIntesity();
		}
	};
	struct cerealCamera
	{
		vec3 position;
		vec2 nearfar;
		vec2 rotation;
		vec3 direction;
		int typeCamera;
		void SetCamera(Camera& camera)
		{
			position = camera.getPosition();
			nearfar.x = camera.getNear();
			nearfar.y = camera.getFar();
			direction = camera.getDirection();
			rotation = camera.getRotation();
			typeCamera = camera.getTypecamera();
		}
	};
}
// SERIALIZATION
template<class Archive>
void serialize(Archive & archive, Awesome::cerealEntity & m) {
	archive(CEREAL_NVP(m.position), CEREAL_NVP(m.scale), CEREAL_NVP(m.rotation), CEREAL_NVP(m.name_));
}
template<class Archive>
void serialize(Archive & archive, Awesome::cerealDirectionalLight & m) {
	archive(CEREAL_NVP(m.position), CEREAL_NVP(m.direction), CEREAL_NVP(m.typeL),
		CEREAL_NVP(m.color), CEREAL_NVP(m.intesity));
}

template <class Archive>
void serialize(Archive& archive, Awesome::cerealSpotLight& m) {
	archive(CEREAL_NVP(m.position), CEREAL_NVP(m.direction), CEREAL_NVP(m.typeL),
		CEREAL_NVP(m.color), CEREAL_NVP(m.cutoff), CEREAL_NVP(m.outercutoff), CEREAL_NVP(m.constant),
		CEREAL_NVP(m.linear),CEREAL_NVP(m.quadratic));
}

template<class Archive>
void serialize(Archive & archive, Awesome::cerealCamera & m) {
	archive(CEREAL_NVP(m.position), CEREAL_NVP(m.nearfar), CEREAL_NVP(m.direction),
		CEREAL_NVP(m.rotation), CEREAL_NVP(m.typeCamera));
}



namespace Awesome {
	template<class Archive> void serialize(Archive& archive, cerealEntity& m)
	{
		archive(CEREAL_NVP(m.position), CEREAL_NVP(m.scale), CEREAL_NVP(m.rotation), CEREAL_NVP(m.name_));
	}

	template<class Archive> void serialize(Archive& archive, cerealDirectionalLight& m)
	{
		archive(CEREAL_NVP(m.position), CEREAL_NVP(m.direction), CEREAL_NVP(m.typeL),
			CEREAL_NVP(m.color), CEREAL_NVP(m.intesity));
	}

	template <class Archive>
	void serialize(Archive& archive, cerealSpotLight& m) {
		archive(CEREAL_NVP(m.position), CEREAL_NVP(m.direction), CEREAL_NVP(m.typeL),
			CEREAL_NVP(m.color), CEREAL_NVP(m.cutoff), CEREAL_NVP(m.outercutoff), CEREAL_NVP(m.constant),
			CEREAL_NVP(m.linear), CEREAL_NVP(m.quadratic));
	}


	template<class Archive>
	void serialize(Archive & archive, cerealCamera & m) {
		archive(CEREAL_NVP(m.position), CEREAL_NVP(m.nearfar), CEREAL_NVP(m.direction),
			CEREAL_NVP(m.rotation), CEREAL_NVP(m.typeCamera));
	}


}


#endif
