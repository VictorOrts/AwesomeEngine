#include "Core/OpenGL/aopengl.h"
#include "../include/deps/GL/glew.h"
#include "Core/engine.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include "Core/Objects/utils.h"
#include <string>

using namespace Awesome;

void AOpenGL::Clear(vec4 clearcolor) {
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(clearcolor.r, clearcolor.g, clearcolor.b, clearcolor.a);
}

void AOpenGL::DrawElements(int32 whoCallme, int32 indices, uint32 vao, uint32 evo) {
	//printf("\n%d %d %d", indices, vao, evo);
	
	//
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, evo);
	int draw[] = {
		GL_POINTS, GL_LINE_STRIP, GL_LINE_LOOP, GL_LINES,
		GL_TRIANGLES//, GL_QUAD_STRIP, GL_QUADS, GL_POLYGON
	};
	/*if (whoCallme == 0) {
		glPolygonMode(GL_FRONT_AND_BACK, wireframeMode_ ? GL_LINE : GL_FILL);
		//glDrawElements(draw[drawMode_], indices, GL_UNSIGNED_INT, (void*)0);
	}
	else {
		glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, (void*)0);
	}*/
	glDrawElements(GL_TRIANGLES, indices, GL_UNSIGNED_INT, (void*)0);
	glBindVertexArray(0);
	glUseProgram(0);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
}

int32 AOpenGL::LoadTexture(uint32 *ut,int16 width, int16 height, int8 type, int8 filter, int8 tex_c, uchar_ptr data) {
	uint32 id_texture;
	glActiveTexture(GL_TEXTURE0 + texCounter_);
	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	int internalFormat[] = {
		GL_NONE,GL_NONE,GL_NONE,
		GL_RGB,GL_RGBA
	};
	int Filtering[] = {
		GL_NEAREST,GL_LINEAR,GL_NEAREST_MIPMAP_NEAREST,
		GL_LINEAR_MIPMAP_NEAREST,GL_NEAREST_MIPMAP_LINEAR,
		GL_LINEAR_MIPMAP_LINEAR
	};
	int TexCoord[] = {
		GL_CLAMP_TO_EDGE,GL_CLAMP_TO_BORDER,GL_MIRRORED_REPEAT,
		GL_REPEAT
	};
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, Filtering[filter]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, Filtering[filter]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, TexCoord[tex_c]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, TexCoord[tex_c]);
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat[type], width, height, 0, internalFormat[type], GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);
	*ut = texCounter_;
	
	//printf("\nT Valores %d %d %d", *ut, texCounter_, id_texture);
	texCounter_++;
	return id_texture;
}

int32 AOpenGL::GenActiveBindTexture() {
	uint32 id_texture_ = 0;
	glGenTextures(1, &id_texture_);
	glActiveTexture(GL_TEXTURE0 + texCounter_);
	glBindTexture(GL_TEXTURE_CUBE_MAP, id_texture_);
	return id_texture_;
}

void AOpenGL::SetParameteri() {
	int TypeTexture[] = {
		GL_TEXTURE_2D,GL_TEXTURE_CUBE_MAP
	};
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	AOpenGL::texCounter_++;
}

void AOpenGL::LoadTextureCM(int16 count, int16 width, int16 height, uchar_ptr list) {
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + count, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, list);
}


int16 AOpenGL::attrib_location(int32 id_program, char_ptr attribName) {
	int16 attrib = glGetAttribLocation(id_program, attribName);

	if (attrib == -1) {
		string tmp;

		tmp = "WARNING::SHADER::ATRRIB_NOT_FOUND " + string(attribName);
		//tmp += attribName + std::to_string(id_program);
		Engine::engine_instance()->add_log(tmp, kLogColors::kColor_Yellow);
	}
	return attrib;
}

int16 AOpenGL::uniform_location(int32 id_program, char_ptr uniformName) {
	int16 uniform = glGetUniformLocation(id_program, uniformName);
	if (uniform == -1) {
		string tmp;
		tmp = "WARNING::SHADER::UNIFORM_NOT_FOUND " + string(uniformName);
		//tmp += uniformName + std::to_string(id_program);
		Engine::engine_instance()->add_log(tmp, kLogColors::kColor_Yellow);
	}
	return uniform;
}

uniform_map AOpenGL::getUniforms(uint32 program) {
	int32 count = 0;
	glGetProgramiv(Engine::engine_instance()->get_render()->program(), GL_ACTIVE_UNIFORMS, &count);
	//glGetProgramInterfaceiv(Engine::engine_instance()->get_render()->program(), GL_UNIFORM, GL_ACTIVE_RESOURCES, &count2);
	//printf("\nCount %d %d", count, count2);

	uniform_map tmpListUniforms;
	for (uint32 i = 0; i < count; i++) {
		int32  size;
		GLenum type;
		char name[30];
		GLsizei  length;
		glGetActiveUniform(program, i, 30, &length, &size, &type, name);
		int16 location = uniform_location(program, name);
		//tmpListUniforms.insert(string("a"), i);
		tmpListUniforms.insert(std::pair<string, uint32>(string(name), location));
		//printf("\n %s , %d", name, i);
	}

	return tmpListUniforms;
}

uint32 AOpenGL::CreateShader(string vertex, string fragment) {
	string vcode = Utils::ReadFromFile(vertex).c_str();
	string fcode = Utils::ReadFromFile(fragment).c_str();
	const char* vertexShaderSource = vcode.c_str();
	const char* fragmentShaderSource = fcode.c_str();

	GLint success;
	char infoLog[512];
	string tmp;
	string tmp2;
	int size = vcode.size();
	//CompileVertex
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, (const GLchar**)&vertexShaderSource, &size);
	glCompileShader(vertexShader);
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		tmp = "ERROR::SHADER::VERTEX::COMPILATION_FAILED ";
		tmp2 = infoLog;
		Engine::engine_instance()->add_log(tmp, kLogColors::kColor_Red);
		Engine::engine_instance()->add_log(tmp2, kLogColors::kColor_Red);
	}

	// Fragment shader

	size = fcode.size();
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentShaderSource, &size);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		tmp.clear();
		tmp2.clear();
		tmp = "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED ";
		tmp2 = infoLog;
		Engine::engine_instance()->add_log(tmp, kLogColors::kColor_Red);
		Engine::engine_instance()->add_log(tmp2, kLogColors::kColor_Red);
	}

	// Create Program and Link shaders
	GLuint program_ = glCreateProgram();
	glAttachShader(program_, vertexShader);
	glAttachShader(program_, fragmentShader);
	glLinkProgram(program_);
	// Check for linking errors
	glGetProgramiv(program_, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program_, 512, NULL, infoLog);
		tmp.clear();
		tmp2.clear();
		tmp = "ERROR::SHADER::PROGRAM::LINKING_FAILED ";
		tmp2 = infoLog;
		Engine::engine_instance()->add_log(tmp, kLogColors::kColor_Red);
		Engine::engine_instance()->add_log(tmp2, kLogColors::kColor_Red);
		printf("\n%s", tmp2.c_str());
	}
	//Delete Shaders and Clear string
	vcode.clear();
	fcode.clear();
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return program_;
}

void AOpenGL::LoadGeometry(uint32 *VAO, uint32 *EVO, uint32 *m_indi, std::vector<float> vertex, std::vector<float>normals, std::vector<float> uv, std::vector<uint32> index) {
	uint32 posVBO;
	uint32 norVBO;
	uint32 uvVBO;

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	glGenBuffers(1, &posVBO);
	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*vertex.size(), &(*vertex.begin()), GL_STATIC_DRAW);
	if (!normals.empty()) {
		glGenBuffers(1, &norVBO);
		glBindBuffer(GL_ARRAY_BUFFER, norVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*normals.size(), &(*normals.begin()), GL_STATIC_DRAW);
	}
	else {
		//printf("\nNormales vacias");
	}

	glGenBuffers(1, &uvVBO);
	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float)*uv.size(), &(*uv.begin()), GL_STATIC_DRAW);

	glGenBuffers(1, EVO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EVO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32)*index.size(), &(*index.begin()), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, posVBO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, uvVBO);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	if (!normals.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, norVBO);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	else {
	//	printf("\nNormales vacias");
	}

	//Unbind all
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void AOpenGL::UseProgram(uint32 pro) {
	glUseProgram(pro);
}

////TODOOO/////

void AOpenGL::setUniform(int16 posicion, const mat4 matrix) {
	glUniformMatrix4fv(posicion, 1, GL_FALSE, &matrix[0][0]);
}
void AOpenGL::setUniform(int16 posicion, const vec3 vec3_) {
	glUniform3f(posicion, vec3_.x, vec3_.y, vec3_.z);
}
void AOpenGL::setUniform(int16 posicion, const vec4 vec4_) {
	glUniform4f(posicion, vec4_.x, vec4_.y, vec4_.z, vec4_.w);
}
//////////////////////////////////////////////////

void AOpenGL::setUniform(int16 position, const double value0) {
	glUniform1d(position, value0);
}

void AOpenGL::setUniform(int16 position, const float value0) {
	glUniform1f(position, value0);
}

void AOpenGL::setUniform(int16 position, const int32 value0) {
	glUniform1i(position, value0);
}

void AOpenGL::setUniform(int16 position, const uint32 value0) {
	glUniform1ui(position, value0);
}

void AOpenGL::setUniform(int16 position, const double value0, const double value1) {
	glUniform2d(position, value0, value1);
}

void AOpenGL::setUniform(int16 position, const float value0, const float value1) {
	glUniform2f(position, value0, value1);
}

void AOpenGL::setUniform(int16 position, const int32 value0, const int32 value1) {
	glUniform2i(position, value0, value1);
}

void AOpenGL::setUniform(int16 position, const uint32 value0, const uint32 value1) {
	glUniform2ui(position, value0, value1);
}

void AOpenGL::setUniform(int16 position, const double value0, const double value1, const double value2) {
	glUniform3d(position, value0, value1, value2);
}

void AOpenGL::setUniform(int16 position, const float value0, const float value1, const float value2) {
	glUniform3f(position, value0, value1, value2);
}

void AOpenGL::setUniform(int16 position, const int32 value0, const int32 value1, const int32 value2) {
	glUniform3i(position, value0, value1, value2);
}

void AOpenGL::setUniform(int16 position, const uint32 value0, const uint32 value1, const uint32 value2) {
	glUniform3ui(position, value0, value1, value2);
}

void AOpenGL::setUniform(int16 position, const double value0, const double value1, const double value2, const double value3) {
	glUniform4d(position, value0, value1, value2, value3);
}

void AOpenGL::setUniform(int16 position, const float value0, const float value1, const float value2, const float value3) {
	glUniform4f(position, value0, value1, value2, value3);
}

void AOpenGL::setUniform(int16 position, const int32 value0, const int32 value1, const int32 value2, const int32 value3) {
	glUniform4i(position, value0, value1, value2, value3);
}

void AOpenGL::setUniform(int16 position, const uint32 value0, const uint32 value1, const uint32 value2, const uint32 value3) {
	glUniform4ui(position, value0, value1, value2, value3);
}

/////////////////////////////////////////////////

void AOpenGL::bindTexture(uint32 tex_,uint32 ut) {
	//glActiveTexture(GL_TEXTURE0 + tex_);
	glBindTexture(GL_TEXTURE_2D, tex_);
}
void AOpenGL::activeTexture(uint32 unit) {
	glActiveTexture(GL_TEXTURE0 + unit);
	//glBindTexture(GL_TEXTURE_2D, tex_);
}




uint32 AOpenGL::CreateNullTexture(uint32 * ut,RenderMaterial type,int16 w, int16 h) {
	uint32 texture;
	glActiveTexture(GL_TEXTURE0 + texCounter_);
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	switch (type)
	{
	case kDiffuse:glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		break;
	case kDepth:glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, w, h, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
		break;
	}
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glBindTexture(GL_TEXTURE_2D, 0);
	*ut = texCounter_;
	//printf("\nNT Valores %d %d %d ", texCounter_, *ut,texture);
	texCounter_++;
	return texture;
}
/////////////FRAMEBUFFERS ////////////

uint32 AOpenGL::CreateFrameBuffer(uint32 texture,RenderMaterial type) {
	uint32 depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	switch (type)
	{
	case kDiffuse:

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);
		break;
	case kDepth: {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, texture, 0);
		glDrawBuffer(GL_NONE);
		glReadBuffer(GL_NONE);
	}
				 break;
	}
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Utils::Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!", kLogColors::kColor_Red);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return depthMapFBO;
}



void AOpenGL::DrawRTT(uint32 prog_, uint32 tex_,uint32 ut) {
	UseProgram(prog_);
	AOpenGL::setUniform(uniform_location(prog_, "tex"), 0);
	//bindTexture(tex_,ut);
	activeTexture(0);
	glBindTexture(GL_TEXTURE_2D, tex_);
	if (Engine::engine_instance()->currentScene()->shadowExam != nullptr) {
		uint32 id_tex = Engine::engine_instance()->currentScene()->shadowExam->get_texture();
		uint32 unit_tex = Engine::engine_instance()->currentScene()->shadowExam->get_unit_texture();
		AOpenGL::setUniform(uniform_location(prog_, "depth"), 1);
		activeTexture(1);
		glBindTexture(GL_TEXTURE_2D, id_tex);
	}
	AOpenGL::setUniform(uniform_location(prog_, "freichen_active"), Engine::engine_instance()->currentScene()->freichen);
	AOpenGL::setUniform(uniform_location(prog_, "gradient_active"), Engine::engine_instance()->currentScene()->gradient);
	//if (Engine::engine_instance()->currentScene()->get_lights().size() > 0) {
		//uint32 id_tex = Engine::engine_instance()->currentScene()->get_lights()[0]->get_Shadow()->get_texture();
		//uint32 unit_tex = Engine::engine_instance()->currentScene()->get_lights()[0]->get_Shadow()->get_unit_texture();
		/*AOpenGL::setUniform(uniform_location(prog_, "shadowTex"), 1);
		activeTexture(1);
		bindTexture(id_tex, unit_tex);*/
	//}
}


void AOpenGL::BindFramebuffer(uint32 fbo,uint32 tex,RenderMaterial type,int16 w, int16 h) {
	
	//glBindTexture(GL_TEXTURE_2D, tex);
	//glBindTexture(GL_TEXTURE_2D, tex);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glViewport(0, 0, w, h);
	switch (type)
	{
	case kDiffuse: 
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		//glClear(GL_DEPTH_BUFFER_BIT);
		//glDisable(GL_CULL_FACE);
		break;
	case kDepth: {
		
		glClear(GL_DEPTH_BUFFER_BIT);
		//glEnable(GL_CULL_FACE);
		//glCullFace(GL_FRONT);
		}
		break;
	}
	//glEnable(GL_DEPTH_TEST);
}

void AOpenGL::UnBindFramebuffer(RenderMaterial type) {
	//glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glViewport(0, 0, Engine::engine_instance()->window()->window_width(), Engine::engine_instance()->window()->window_height());
	switch (type)
	{
	case kDiffuse: {
		//glClear(GL_COLOR_BUFFER_BIT);
		glDisable(GL_DEPTH_TEST);
	}
		break;
	case kDepth: {
		//glCullFace(GL_BACK);
	}
		break;
	}

}

uint32 AOpenGL::CreateRenderBuffer(uint32 fbo,int tipo) {
	uint32 rbo;
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	if (tipo == 0){
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}
	else {
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rbo);
	}

																								 
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		Utils::Log("ERROR::FRAMEBUFFER:: Framebuffer is not complete!", kLogColors::kColor_Red);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return rbo;
}

void AOpenGL::DeleteBuffers(uint32 buff) {
	glDeleteBuffers(1, &buff);
}
void AOpenGL::DeleteProgram(uint32 pro) {
	glUseProgram(0);
	glDeleteProgram(pro);
}

void AOpenGL::setDrawMode(int dm) {
	drawMode_ = dm;
}

void AOpenGL::setWireframMode() {
	wireframeMode_ = !wireframeMode_;
}


void AOpenGL::SetViewport(int32 w, int32 h) {
	glViewport(0, 0, w, h);
}
void AOpenGL::ClearDepthBuffer() {
	glClear(GL_DEPTH_BUFFER_BIT);
}


//DEFERRED
uint32 AOpenGL::FramebufferDeferred() {
	uint32 id_framebuffer;
	glGenFramebuffers(1, &id_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, id_framebuffer);
	return id_framebuffer;
}

uint32 AOpenGL::TextureDeferred(int contador, int tipo, int w, int h) {


	uint32 id_texture;
	glGenTextures(1, &id_texture);
	glBindTexture(GL_TEXTURE_2D, id_texture);
	if (tipo == 0 || tipo == 1) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, w, h, 0, GL_RGB, GL_FLOAT, NULL);
	}
	else
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_FLOAT, NULL);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	texCounter_++;
	return id_texture;
}

void AOpenGL::attachmentDeferred(std::map<uint32, string> * mapTextures,uint32 framebuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	int i = 0;
	for (auto& it : *mapTextures) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D,it.first, 0);
		Utils::Log(string(it.second + std::to_string(it.first)),kLogColors::kColor_Watermarine,kLogOutput::kWindow_CMD);
	}
	uint32 attach[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attach);
	
}

void AOpenGL::bindFramebufferDeferred(uint32 framebuffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	if (framebuffer > 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

}