#include "Core\Objects\deferred_implemetantion.h"
#include "Core\OpenGL\aopengl.h"

using namespace Awesome;


Gbuffer::Gbuffer() {

}

void Gbuffer::InitDeferred() {
	int16 width = Engine::engine_instance()->window()->window_width();
	int16 height = Engine::engine_instance()->window()->window_height();
	string gbuffer_names[3] = { "Position","Normals","AlbedoSpec" };
	framebuffer_id_ = AOpenGL::FramebufferDeferred();
	for (int i = 0; i < Gbuffer_types::kGbuffer_Size; i++){
		uint32 id_texture = AOpenGL::TextureDeferred(i, Gbuffer_types(i), width, height);
		textures_.insert(std::pair<uint32, string>(id_texture, gbuffer_names[i]));
	}
	Utils::Log(gbuffer_names[0], kLogColors::kColor_Watermarine, kLogOutput::kWindow_CMD);
	Utils::Log(gbuffer_names[1], kLogColors::kColor_Watermarine, kLogOutput::kWindow_CMD);
	Utils::Log(gbuffer_names[2], kLogColors::kColor_Watermarine, kLogOutput::kWindow_CMD);

	AOpenGL::attachmentDeferred(&textures_, framebuffer_id_);
	renderbuffer_id_ = AOpenGL::CreateRenderBuffer(framebuffer_id_, 1);
	gbuffer_names->clear();
}

uint32 Gbuffer::framebuffer() {
	return framebuffer_id_;
}
std::map<uint32, string> Gbuffer::textures() {
	return textures_;
}

void Gbuffer::bindFramebuffer() {
	AOpenGL::bindFramebufferDeferred(framebuffer_id_);
}

void Gbuffer::unbindFramebuffer() {
	AOpenGL::bindFramebufferDeferred(0);
}

Gbuffer::~Gbuffer(){

}