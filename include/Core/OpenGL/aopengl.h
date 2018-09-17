/*
Functions of OpenGL

by Victor Orts
*/

#ifndef __AOPENGL_H__
#define __AOPENGL_H__
#include "../include/Core/types.h"
#include <vector>
namespace Awesome {
	namespace AOpenGL {
		void Clear(vec4 clearcolor);

		void DrawElements(int32 whoCallme, int32 indices, uint32 vao, uint32 evo);
		int32 LoadTexture(uint32 *ut ,int16 width, int16 height, int8 type, int8 filter, int8 tex_c, uchar_ptr data);
		int32 GenActiveBindTexture();
		void SetParameteri();
		void LoadTextureCM(int16 count, int16 width, int16 height, uchar_ptr list);
		uint32 CreateShader(string vertex, string fragment);
		void LoadGeometry(uint32 *VAO, uint32 *EVO, uint32 *m_indi, std::vector<float>, std::vector<float>, std::vector<float>, std::vector<uint32> index);
		void UseProgram(uint32 pro);
		int16 attrib_location(int32 program, char_ptr attribname);
		int16 uniform_location(int32 program, char_ptr uniformbname);
		void bindTexture(uint32 tex_,uint32 ut);
		void activeTexture(uint32 unit);
		uniform_map getUniforms(uint32 program);
		/*	Overload method of set uniform to vec1 to vec4  also mat4
		*	---------------------------------------------------------
		*	Future add vec3;
		*/
		void setUniform(int16 position, const mat4 mat4_);
		void setUniform(int16 position, const vec3 vec3_);
		void setUniform(int16 position, const vec4 vec4_);

		void setUniform(int16 position, const double value0);
		void setUniform(int16 position, const float value0);
		void setUniform(int16 position, const int32 value0);
		void setUniform(int16 position, const uint32 value0);

		void setUniform(int16 position, const double value0, const double value1);
		void setUniform(int16 position, const float value0, const float value1);
		void setUniform(int16 position, const int32 value0, const int32 value1);
		void setUniform(int16 position, const uint32 value0, const uint32 value1);

		void setUniform(int16 position, const double value0, const double value1, const double value2);
		void setUniform(int16 position, const float value0, const float value1, const float value2);
		void setUniform(int16 position, const int32 value0, const int32 value1, const int32 value2);
		void setUniform(int16 position, const uint32 value0, const uint32 value1, const uint32 value2);

		void setUniform(int16 position, const double value0, const double value1, const double value2, const double value3);
		void setUniform(int16 position, const float value0, const float value1, const float value2, const float value3);
		void setUniform(int16 position, const int32 value0, const int32 value1, const int32 value2, const int32 value3);
		void setUniform(int16 position, const uint32 value0, const uint32 value1, const uint32 value2, const uint32 value3);
		//why not 0? because the gui image take the number 0.
		static int texCounter_ = 1;
		static int drawMode_ = 4;
		static bool wireframeMode_ = false;
		void setDrawMode(int a);
		void setWireframMode();
		//FrameBuffer Functions
		uint32 CreateNullTexture(uint32 * ut,RenderMaterial type,int16 ,int16);
		void DrawRTT(uint32 prog_, uint32 tex_,uint32 ut);
		uint32 CreateFrameBuffer(uint32 texture,RenderMaterial type);
		void SetViewport(int32, int32);
		void BindFramebuffer(uint32 ,uint32 ,RenderMaterial,int16 ,int16);
		void UnBindFramebuffer(RenderMaterial);
		uint32 CreateRenderBuffer(uint32 fbo_,int tipo=0);
		void ClearDepthBuffer();
		//Delete and clean
		void DeleteBuffers(uint32 buff);
		void DeleteProgram(uint32 pro);

		//DEFERRED FUNCTIONS
		uint32 FramebufferDeferred();
		uint32 TextureDeferred(int contador, int tipo, int w, int h);
		void attachmentDeferred(std::map<uint32, string> *tipo,uint32 frambuffer);
		void bindFramebufferDeferred(uint32 frame_);
		
	}
}
#endif // !__H_AOPENGL__
