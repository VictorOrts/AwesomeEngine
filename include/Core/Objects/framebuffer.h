#ifndef FRAME_BUFFER_H_
#define FRAME_BUFFER_H_

#include "Core/types.h"
#include "texture.h"
namespace Awesome {
	class FrameBuffer {
	public:

		FrameBuffer();
		~FrameBuffer();
		virtual void begin()=0;
		virtual void end() = 0;
		void initFrameBuffer(RenderMaterial type,int16 , int16);
		uint32 get_fbo();
		uint32 get_rbo();
		uint32 get_texture();
		uint32 get_unit_texture();
		void Bind();
		void unBind();

	private:
		int16 width_;
		int16 height_;
		uint32 fbo_;
		uint32 rbo_;
		uint32  texture_;
		uint32 unit_texture_;
		RenderMaterial type_frame_;
	};
}

#endif // FRAME_BUFFER_H_
