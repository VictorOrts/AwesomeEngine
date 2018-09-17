#include "Core\Objects\texture.h"
#include "Core\OpenGL\aopengl.h"
#include "../include/deps/GL/glew.h"
#include "Core\engine.h"
#include "Core\Task\texturetask.h"
#define STBI_FAILURE_USERMSG
#define STB_IMAGE_IMPLEMENTATION
#include "external\stb_image.h"

using namespace Awesome;

Texture::Texture(string name) {
	width_ = 0;
	height_ = 0;
	type_ = 0;
	id_texture_ = 0;
	texture_unit_ = 0;
	name_ = name;
	loaded = false;
	if (name != "blanco.jpg") {
		//id_texture_ = Engine::engine_instance()->defaults()->default_texture()->id_texture();
		//Engine::engine_instance()->get_task()->AddJob(std::make_shared<TextureTask>(this));
		loadTexture(name_);
	}

	
	//Engine::engine_instance()->get_task()->AddJob(std::make_shared<TextureTask>(this));
}

Texture::Texture() {
	id_texture_ = 0;
	texture_unit_ = 0;
}
Texture::~Texture() {
}

int16 Texture::width() {
	return width_;
}

int16 Texture::height() {
	return height_;
}
uint32 Texture::id_texture() {
	return id_texture_;
}

uint32 Texture::texture_unit() {
	return texture_unit_;
}
string Texture::name() {
	return name_;
}

void Texture::set_name(string file_) {
	name_ = file_;
}
void Texture::loadTexture(string fileName, TypeFiltering filter, TypeTexCoord texcoord) {
	name_ = fileName;
	filtering_ = filter;
	tex_coord_ = texcoord;
	load();
	if (loaded == true) {
		loadToGPU();
	}
	//return id_texture;
}
void Texture::loadTexture(std::vector<string> filename, TypeFiltering filter, TypeTexCoord texcoord) {
	name_ = "Cubemap";
	filtering_ = filter;
	tex_coord_ = texcoord;

	/*glGenTextures(1, &id_texture_);
	glActiveTexture(GL_TEXTURE0+AOpenGL::texCounter_);

	glBindTexture(GL_TEXTURE_CUBE_MAP, id_texture_);*/
	id_texture_ = AOpenGL::GenActiveBindTexture();
	for (GLuint i = 0; i < filename.size(); i++)
	{
		int w, h, type;
		uchar_ptr image = stbi_load(filename[i].c_str(), &w, &h, &type, 0);
		AOpenGL::LoadTextureCM(i, w, h, image);
		//glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0,GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		STBI_FREE(image);
	}
	AOpenGL::SetParameteri();
}
void Texture::load() {
	int w, h, type;
	image_ = stbi_load(name_.c_str(), &w, &h, &type, 0);
	if (!image_) throw std::runtime_error(stbi_failure_reason());
	width_ = w;
	height_ = h;
	type_ = type;

	loaded = true;
}

void Texture::loadToGPU() {
	id_texture_ = AOpenGL::LoadTexture(&texture_unit_, width_, height_, TypeTexture(type_), TypeFiltering(filtering_), TypeTexCoord(tex_coord_), image_);
	STBI_FREE(image_);
}