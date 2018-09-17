#include "Core\Objects\rendertotexture.h"
#include "Core\OpenGL\aopengl.h"
using namespace Awesome;

RenderToTexture::RenderToTexture(int16 w ,int16 h) {
	initFrameBuffer(RenderMaterial::kDiffuse,w,h);
	plane_ = std::make_shared<Plane>(Plane::kPlane_RTT);
	plane_->Create();
	id_prog_ = AOpenGL::CreateShader("vertexShaderRTT.glsl", "fragmentShaderRTT.glsl");
	degradado_ = 0;

}

void RenderToTexture::rellenaMap() {
	for (int i = 0; i < PostProcess::Count; i++) {
		valuesPP.insert(std::pair<PostProcess, int8>(PostProcess(i), 0));
	}
}

template<>
void RenderToTexture::setEffect<kDegradado>(bool active) {
	valuesPP.find(kDegradado)->second = active;
}
template<>
void RenderToTexture::setEffect<kWyB>(bool active) {
	valuesPP.find(kWyB)->second = active;
}

RenderToTexture::~RenderToTexture()
{
}

void RenderToTexture::begin() {
	Bind();
}

void RenderToTexture::end() {
	unBind();
	drawRenderToTexture();
}
void RenderToTexture::drawRenderToTexture() {
	AOpenGL::DrawRTT(id_prog_, get_texture(),get_unit_texture());
	AOpenGL::DrawElements(1, plane_->get_numindices(), plane_->get_vao(), plane_->get_evo());
}