#include "Core/Objects/shadowtexture.h"
#include "Core\OpenGL\aopengl.h"
#include "Core\engine.h"

using namespace Awesome;

ShadowTexture::ShadowTexture(int16 w, int16 h) {
	initFrameBuffer(RenderMaterial::kDepth,w,h);
}

ShadowTexture::~ShadowTexture() {
}

void ShadowTexture::begin() {
	Bind();
}

void ShadowTexture::end() {
	unBind();
}
