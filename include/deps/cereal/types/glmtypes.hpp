/*  Templates of GLM
 *	Templates of vec3 vec4 and mat4x4 for save my scene with cereal.
 * 	by Victor Orts
 */

#ifndef _H_GLM_TYPES_CEREAL_
#define _H_GLM_TYPES_CEREAL_

#include <deps\cereal\cereal.hpp>
#include <deps\glm\glm.hpp>


template<typename Archive> void serialize(Archive& archive, glm::vec2& v2)
{
	archive(cereal::make_nvp("x", v2.x), cereal::make_nvp("y", v2.y));
}

namespace glm {
	template<typename Archive> void serialize(Archive& archive, glm::vec2& v2)
	{
		archive(cereal::make_nvp("x", v2.x), cereal::make_nvp("y", v2.y));
	}
}

 // Template vec3
 //
 // @param      archive  The archive
 // @param      v3       glm::vector3
 //
 // @tparam     Archive  { description }
 //
template<typename Archive> void serialize(Archive& archive, glm::vec3& v3)
{
	archive(cereal::make_nvp("x", v3.x), cereal::make_nvp("y", v3.y), cereal::make_nvp("z", v3.z));
}

namespace glm {
	template<typename Archive> void serialize(Archive& archive, glm::vec3& v3)
	{
		archive(cereal::make_nvp("x", v3.x), cereal::make_nvp("y", v3.y), cereal::make_nvp("z", v3.z));
	}
}
//End template vec3

// Template vec4
//
// @param      archive  The archive
// @param      v4       glm::vec4
//
// @tparam     Archive  { description }
//
template<typename Archive> void serialize(Archive& archive, glm::vec4& v4)
{
	archive(cereal::make_nvp("x", v4.x), cereal::make_nvp("y", v4.y), cereal::make_nvp("z", v4.z), cereal::make_nvp("w", v4.w));
}

namespace glm {
	template<typename Archive> void serialize(Archive& archive, glm::vec4& v4)
	{
		archive(cereal::make_nvp("x", v4.x), cereal::make_nvp("y", v4.y), cereal::make_nvp("z", v4.z), cereal::make_nvp("w", v4.w));
	}
}

// End template vec4 Template mat4
//
// @param      archive  The archive
// @param      m4       glm::mat4x4
//
// @tparam     Archive  { description }
//
template<typename Archive> void serialize(Archive& archive, glm::mat4& m4)
{
	archive(cereal::make_nvp("v1.x", m4[0].x), cereal::make_nvp("v1.y", m4[0].y), cereal::make_nvp("v1.z", m4[0].z), cereal::make_nvp("v1.w", m4[0].w),
		cereal::make_nvp("v2.x", m4[1].x), cereal::make_nvp("v2.y", m4[1].y), cereal::make_nvp("v2.z", m4[1].z), cereal::make_nvp("v2.w", m4[1].w),
		cereal::make_nvp("v3.x", m4[2].x), cereal::make_nvp("v3.y", m4[2].y), cereal::make_nvp("v3.z", m4[2].z), cereal::make_nvp("v3.w", m4[2].w),
		cereal::make_nvp("v4.x", m4[3].x), cereal::make_nvp("v4.y", m4[3].y), cereal::make_nvp("v4.z", m4[3].z), cereal::make_nvp("v4.w", m4[3].w)
	);
}

namespace glm {
	template<typename Archive> void serialize(Archive& archive, glm::mat4& m4)
	{
		archive(cereal::make_nvp("v1.x", m4[0].x), cereal::make_nvp("v1.y", m4[0].y), cereal::make_nvp("v1.z", m4[0].z), cereal::make_nvp("v1.w", m4[0].w),
			cereal::make_nvp("v2.x", m4[1].x), cereal::make_nvp("v2.y", m4[1].y), cereal::make_nvp("v2.z", m4[1].z), cereal::make_nvp("v2.w", m4[1].w),
			cereal::make_nvp("v3.x", m4[2].x), cereal::make_nvp("v3.y", m4[2].y), cereal::make_nvp("v3.z", m4[2].z), cereal::make_nvp("v3.w", m4[2].w),
			cereal::make_nvp("v4.x", m4[3].x), cereal::make_nvp("v4.y", m4[3].y), cereal::make_nvp("v4.z", m4[3].z), cereal::make_nvp("v4.w", m4[3].w)
		);
	}
}
//End template mat4

#endif //!_H_GLM_TYPES_CEREAL_