/*
*	Class Types
*	Types of the engine
*	by Victor Orts
*/

#ifndef __TYPES_H__
#define __TYPES_H__

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <map>
#include "deps/glm/glm.hpp"
#include "deps/glm/vec2.hpp"
#include "deps/glm/vec3.hpp"
#include "deps/glm/vec4.hpp"
#include "deps/glm/gtc/matrix_transform.hpp"
#include "deps/glm/gtc/type_ptr.hpp"
//Cereal

#include <deps/cereal/cereal.hpp>
#include <deps/cereal/archives/binary.hpp>
#include <deps/cereal/archives/portable_binary.hpp>
#include <deps/cereal/archives/json.hpp>

#include <deps/cereal/types/string.hpp>
#include <deps/cereal/types/utility.hpp>
#include <deps/cereal/types/memory.hpp>
#include <deps/cereal/types/complex.hpp>
#include <deps/cereal/types/base_class.hpp>
#include <deps/cereal/types/array.hpp>
#include <deps/cereal/types/vector.hpp>
#include <deps/cereal/types/map.hpp>
#include <deps/cereal/types/utility.hpp>
#include <deps/cereal/types/bitset.hpp>
#include <deps/cereal/types/polymorphic.hpp>
#include <deps/cereal/types/glmtypes.hpp>

#include <fstream>
#include <thread>
enum RenderMaterial {
	kDiffuse,
	kNormal,
	kDepth,
	kSpecular,
};
enum LightType {
	kDirLight,
	kPointLight,
	kSpotLight,
	kNone
};

enum uniformLight {
	uType=0,
	uPosition,
	uDirection,
	uColor,
	uIntesity,
	uConstant,
	uLinear,
	uQuadratic,
	uCutoff,
	uOuterCuttof,
	uLightPos,
	uLightSpaceMatrix,
	uShadowmap
};
enum uniformEntity {
	uDiffuse_tex=0,
	uSpecular_tex,
	uCurrentLight,
	uModel,
	uViewproject,
	uCameraPos,
	uMatAmbient,
	uMatDiffuse,
	uMatSpecular,
	uMatShininess
};

#define PI 3.1415926f
#define MAX_LIGHT 10

#ifdef WINDOWS_PLATFORM
const char kNoneValueSigned = -1;
typedef ::int8_t int8;
typedef ::int16_t int16;
typedef ::int32_t int32;
typedef ::int64_t int64;
typedef ::uint8_t uint8;
typedef ::uint16_t uint16;
typedef ::uint32_t uint32;
typedef ::uint64_t uint64;
typedef unsigned char * uchar_ptr;
typedef const char* char_ptr;
typedef std::string string;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;
typedef glm::ivec3 ivec3;
typedef std::map<string, uint32> uniform_map;
typedef std::map<string, uint32> uniform_map;
typedef std::map<string, ivec3> logGui;
typedef std::map<int8, uint32> uniform_faster_map;
#elif LINUX_PLATFORM
const char kNoneValueSigned = -1;
typedef ::int8_t int8;
typedef ::int16_t int16;
typedef ::int32_t int32;
typedef ::uint8_t uint8;
typedef ::uint16_t uint16;
typedef ::uint32_t uint32;
typedef const char* char_ptr;
typedef std::string string;
typedef glm::vec2 vec2;
typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat4 mat4;
#endif // WINDOWS_PLATFORM

#endif // !__H_TYPES__
