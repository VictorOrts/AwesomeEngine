#include "Core\Objects\framebuffer.h"
#include "Core\OpenGL\aopengl.h"

using namespace Awesome;

FrameBuffer::FrameBuffer() {

}

FrameBuffer::~FrameBuffer() {
}

void FrameBuffer::initFrameBuffer(RenderMaterial type,int16 w,int16 h) {
	type_frame_ = type;
	width_ = w;
	height_ = h;
	texture_ = AOpenGL::CreateNullTexture(&unit_texture_, type_frame_,width_,height_);
	fbo_ = AOpenGL::CreateFrameBuffer(texture_,type_frame_);
	if (type == RenderMaterial::kDiffuse) {
		rbo_ = AOpenGL::CreateRenderBuffer(fbo_);
	}


}
uint32 FrameBuffer::get_fbo() {
	return fbo_;
}
uint32 FrameBuffer::get_rbo() {
	return rbo_;
}

uint32 FrameBuffer::get_texture() {
	return texture_;
}
uint32 FrameBuffer::get_unit_texture() {
	return unit_texture_;
}

void FrameBuffer::Bind() {
	AOpenGL::BindFramebuffer(fbo_,texture_,type_frame_,width_,height_);
}
void FrameBuffer::unBind() {
	AOpenGL::UnBindFramebuffer(type_frame_);
}