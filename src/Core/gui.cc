#include "Core/gui.h"
#include "Core/engine.h"
#include "Core\Components\rendercomponent.h"
// 512 1024  128 1024
#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION

#include "Core\Objects\utils.h"
#include "deps/nuklear/nuklear.h"
#include "deps/nuklear/style.c"
#include <time.h>
#include <iostream>
#include "../include/deps/GL/glew.h"
#include "../include/deps/GLFW/glfw3.h"

using namespace Awesome;

enum nk_glfw_init_state {
	NK_GLFW3_DEFAULT = 0,
	NK_GLFW3_INSTALL_CALLBACKS
};

NK_API struct nk_context*   nk_glfw3_init(GLFWwindow *win, enum nk_glfw_init_state);
NK_API void                 nk_glfw3_shutdown(void);
NK_API void                 nk_glfw3_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_glfw3_font_stash_end(void);
NK_API void                 nk_glfw3_new_frame(void);
NK_API void                 nk_glfw3_render(enum nk_anti_aliasing, int max_vertex_buffer, int max_element_buffer);

NK_API void                 nk_glfw3_device_destroy(void);
NK_API void                 nk_glfw3_device_create(void);

NK_API void                 nk_glfw3_char_callback(GLFWwindow *win, unsigned int codepoint);
NK_API void                 nk_gflw3_scroll_callback(GLFWwindow *win, double xoff, double yoff);

#ifndef NK_GLFW_TEXT_MAX
#define NK_GLFW_TEXT_MAX 256
#endif

struct nk_glfw_device {
	struct nk_buffer cmds;
	struct nk_draw_null_texture null;
	GLuint vbo, vao, ebo;
	GLuint prog;
	GLuint vert_shdr;
	GLuint frag_shdr;
	GLint attrib_pos;
	GLint attrib_uv;
	GLint attrib_col;
	GLint uniform_tex;
	GLint uniform_proj;
	GLuint font_tex;
};

struct nk_glfw_vertex {
	float position[2];
	float uv[2];
	nk_byte col[4];
};

static struct nk_glfw {
	GLFWwindow *win;
	int width, height;
	int display_width, display_height;
	struct nk_glfw_device ogl;
	struct nk_context ctx;
	struct nk_font_atlas atlas;
	struct nk_vec2 fb_scale;
	unsigned int text[NK_GLFW_TEXT_MAX];
	int text_len;
	float scroll;
} glfw;

#define NK_SHADER_VERSION "#version 300 es\n"

NK_API void
nk_glfw3_device_create(void)
{
	GLint status;
	static const GLchar *vertex_shader =
		NK_SHADER_VERSION
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 TexCoord;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main() {\n"
		"   Frag_UV = TexCoord;\n"
		"   Frag_Color = Color;\n"
		"   gl_Position = ProjMtx * vec4(Position.xy, 0, 1);\n"
		"}\n";
	static const GLchar *fragment_shader =
		NK_SHADER_VERSION
		"precision mediump float;\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main(){\n"
		"   Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
		"}\n";

	struct nk_glfw_device *dev = &glfw.ogl;
	nk_buffer_init_default(&dev->cmds);
	dev->prog = glCreateProgram();
	dev->vert_shdr = glCreateShader(GL_VERTEX_SHADER);
	dev->frag_shdr = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(dev->vert_shdr, 1, &vertex_shader, 0);
	glShaderSource(dev->frag_shdr, 1, &fragment_shader, 0);
	glCompileShader(dev->vert_shdr);
	glCompileShader(dev->frag_shdr);
	glGetShaderiv(dev->vert_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glGetShaderiv(dev->frag_shdr, GL_COMPILE_STATUS, &status);
	assert(status == GL_TRUE);
	glAttachShader(dev->prog, dev->vert_shdr);
	glAttachShader(dev->prog, dev->frag_shdr);
	glLinkProgram(dev->prog);
	glGetProgramiv(dev->prog, GL_LINK_STATUS, &status);
	assert(status == GL_TRUE);

	dev->uniform_tex = glGetUniformLocation(dev->prog, "Texture");
	dev->uniform_proj = glGetUniformLocation(dev->prog, "ProjMtx");
	dev->attrib_pos = glGetAttribLocation(dev->prog, "Position");
	dev->attrib_uv = glGetAttribLocation(dev->prog, "TexCoord");
	dev->attrib_col = glGetAttribLocation(dev->prog, "Color");

	{
		/* buffer setup */
		GLsizei vs = sizeof(struct nk_glfw_vertex);
		size_t vp = offsetof(struct nk_glfw_vertex, position);
		size_t vt = offsetof(struct nk_glfw_vertex, uv);
		size_t vc = offsetof(struct nk_glfw_vertex, col);

		glGenBuffers(1, &dev->vbo);
		glGenBuffers(1, &dev->ebo);
		glGenVertexArrays(1, &dev->vao);

		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glEnableVertexAttribArray((GLuint)dev->attrib_pos);
		glEnableVertexAttribArray((GLuint)dev->attrib_uv);
		glEnableVertexAttribArray((GLuint)dev->attrib_col);

		glVertexAttribPointer((GLuint)dev->attrib_pos, 2, GL_FLOAT, GL_FALSE, vs, (void*)vp);
		glVertexAttribPointer((GLuint)dev->attrib_uv, 2, GL_FLOAT, GL_FALSE, vs, (void*)vt);
		glVertexAttribPointer((GLuint)dev->attrib_col, 4, GL_UNSIGNED_BYTE, GL_TRUE, vs, (void*)vc);
	}

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

NK_INTERN void
nk_glfw3_device_upload_atlas(const void *image, int width, int height)
{
	struct nk_glfw_device *dev = &glfw.ogl;
	glGenTextures(1, &dev->font_tex);
	glBindTexture(GL_TEXTURE_2D, dev->font_tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
		GL_RGBA, GL_UNSIGNED_BYTE, image);
}

NK_API void
nk_glfw3_device_destroy(void)
{
	struct nk_glfw_device *dev = &glfw.ogl;
	glDetachShader(dev->prog, dev->vert_shdr);
	glDetachShader(dev->prog, dev->frag_shdr);
	glDeleteShader(dev->vert_shdr);
	glDeleteShader(dev->frag_shdr);
	glDeleteProgram(dev->prog);
	glDeleteTextures(1, &dev->font_tex);
	glDeleteBuffers(1, &dev->vbo);
	glDeleteBuffers(1, &dev->ebo);
	nk_buffer_free(&dev->cmds);
}

NK_API void
nk_glfw3_render(enum nk_anti_aliasing AA, int max_vertex_buffer, int max_element_buffer)
{
	struct nk_glfw_device *dev = &glfw.ogl;
	GLfloat ortho[4][4] = {
		{ 2.0f, 0.0f, 0.0f, 0.0f },
		{ 0.0f,-2.0f, 0.0f, 0.0f },
		{ 0.0f, 0.0f,-1.0f, 0.0f },
		{ -1.0f,1.0f, 0.0f, 1.0f },
	};
	ortho[0][0] /= (GLfloat)glfw.width;
	ortho[1][1] /= (GLfloat)glfw.height;

	/* setup global state */
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	/* setup program */
	glUseProgram(dev->prog);
	glUniform1i(dev->uniform_tex, 0);
	glUniformMatrix4fv(dev->uniform_proj, 1, GL_FALSE, &ortho[0][0]);
	glViewport(0, 0, (GLsizei)glfw.display_width, (GLsizei)glfw.display_height);
	{
		/* convert from command queue into draw list and draw to screen */
		const struct nk_draw_command *cmd;
		void *vertices, *elements;
		const nk_draw_index *offset = NULL;

		/* allocate vertex and element buffer */
		glBindVertexArray(dev->vao);
		glBindBuffer(GL_ARRAY_BUFFER, dev->vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, dev->ebo);

		glBufferData(GL_ARRAY_BUFFER, max_vertex_buffer, NULL, GL_STREAM_DRAW);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, max_element_buffer, NULL, GL_STREAM_DRAW);

		/* load draw vertices & elements directly into vertex + element buffer */
		vertices = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
		elements = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
		{
			/* fill convert configuration */
			struct nk_convert_config config;
			static const struct nk_draw_vertex_layout_element vertex_layout[] = {
				{ NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, position) },
				{ NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct nk_glfw_vertex, uv) },
				{ NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct nk_glfw_vertex, col) },
				{ NK_VERTEX_LAYOUT_END }
			};
			NK_MEMSET(&config, 0, sizeof(config));
			config.vertex_layout = vertex_layout;
			config.vertex_size = sizeof(struct nk_glfw_vertex);
			config.vertex_alignment = NK_ALIGNOF(struct nk_glfw_vertex);
			config.null = dev->null;
			config.circle_segment_count = 22;
			config.curve_segment_count = 22;
			config.arc_segment_count = 22;
			config.global_alpha = 1.0f;
			config.shape_AA = AA;
			config.line_AA = AA;

			/* setup buffers to load vertices and elements */
			{struct nk_buffer vbuf, ebuf;
			nk_buffer_init_fixed(&vbuf, vertices, (size_t)max_vertex_buffer);
			nk_buffer_init_fixed(&ebuf, elements, (size_t)max_element_buffer);
			nk_convert(&glfw.ctx, &dev->cmds, &vbuf, &ebuf, &config); }
		}
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

		/* iterate over and execute each draw command */
		nk_draw_foreach(cmd, &glfw.ctx, &dev->cmds)
		{
			if (!cmd->elem_count) continue;
			glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
			glScissor(
				(GLint)(cmd->clip_rect.x * glfw.fb_scale.x),
				(GLint)((glfw.height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * glfw.fb_scale.y),
				(GLint)(cmd->clip_rect.w * glfw.fb_scale.x),
				(GLint)(cmd->clip_rect.h * glfw.fb_scale.y));
			glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
			offset += cmd->elem_count;
		}
		nk_clear(&glfw.ctx);
	}

	/* default OpenGL state */
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	//Restart
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glDisable(GL_BLEND);
	glDisable(GL_SCISSOR_TEST);
}

NK_API void
nk_glfw3_char_callback(GLFWwindow *win, unsigned int codepoint)
{
	(void)win;
	if (glfw.text_len < NK_GLFW_TEXT_MAX)
		glfw.text[glfw.text_len++] = codepoint;
}

NK_API void
nk_gflw3_scroll_callback(GLFWwindow *win, double xoff, double yoff)
{
	(void)win; (void)xoff;
	glfw.scroll += (float)yoff;
}

NK_INTERN void
nk_glfw3_clipbard_paste(nk_handle usr, struct nk_text_edit *edit)
{
	const char *text = glfwGetClipboardString(glfw.win);
	if (text) nk_textedit_paste(edit, text, nk_strlen(text));
	(void)usr;
}

NK_INTERN void
nk_glfw3_clipbard_copy(nk_handle usr, const char *text, int len)
{
	char *str = 0;
	(void)usr;
	if (!len) return;
	str = (char*)malloc((size_t)len + 1);
	if (!str) return;
	memcpy(str, text, (size_t)len);
	str[len] = '\0';
	glfwSetClipboardString(glfw.win, str);
	free(str);
}

NK_API struct nk_context*
nk_glfw3_init(GLFWwindow *win, enum nk_glfw_init_state init_state)
{
	glfw.win = win;
	if (init_state == NK_GLFW3_INSTALL_CALLBACKS) {
		glfwSetScrollCallback(win, nk_gflw3_scroll_callback);
		glfwSetCharCallback(win, nk_glfw3_char_callback);
	}

	nk_init_default(&glfw.ctx, 0);
	glfw.ctx.clip.copy = nk_glfw3_clipbard_copy;
	glfw.ctx.clip.paste = nk_glfw3_clipbard_paste;
	glfw.ctx.clip.userdata = nk_handle_ptr(0);
	nk_glfw3_device_create();
	return &glfw.ctx;
}

NK_API void
nk_glfw3_font_stash_begin(struct nk_font_atlas **atlas)
{
	nk_font_atlas_init_default(&glfw.atlas);
	nk_font_atlas_begin(&glfw.atlas);
	*atlas = &glfw.atlas;
}


NK_API void
nk_glfw3_font_stash_end(void)
{
	const void *image; int w, h;
	image = nk_font_atlas_bake(&glfw.atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
	nk_glfw3_device_upload_atlas(image, w, h);
	nk_font_atlas_end(&glfw.atlas, nk_handle_id((int)glfw.ogl.font_tex), &glfw.ogl.null);
	if (glfw.atlas.default_font)
		nk_style_set_font(&glfw.ctx, &glfw.atlas.default_font->handle);
}

NK_API void
nk_glfw3_new_frame(void)
{
	int i;
	double x, y;
	struct nk_context *ctx = &glfw.ctx;
	struct GLFWwindow *win = glfw.win;

	glfwGetWindowSize(win, &glfw.width, &glfw.height);
	glfwGetFramebufferSize(win, &glfw.display_width, &glfw.display_height);
	glfw.fb_scale.x = (float)glfw.display_width / (float)glfw.width;
	glfw.fb_scale.y = (float)glfw.display_height / (float)glfw.height;

	nk_input_begin(ctx);
	for (i = 0; i < glfw.text_len; ++i)
		nk_input_unicode(ctx, glfw.text[i]);

	/* optional grabbing behavior */
	if (ctx->input.mouse.grab)
		glfwSetInputMode(glfw.win, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
	else if (ctx->input.mouse.ungrab)
		glfwSetInputMode(glfw.win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TEXT_START, glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_TEXT_END, glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_START, glfwGetKey(win, GLFW_KEY_HOME) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_END, glfwGetKey(win, GLFW_KEY_END) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_DOWN, glfwGetKey(win, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SCROLL_UP, glfwGetKey(win, GLFW_KEY_PAGE_UP) == GLFW_PRESS);
	nk_input_key(ctx, NK_KEY_SHIFT, glfwGetKey(win, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS ||
		glfwGetKey(win, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS);

	if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
		glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
		nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_V) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_UNDO, glfwGetKey(win, GLFW_KEY_Z) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_REDO, glfwGetKey(win, GLFW_KEY_R) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_WORD_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_WORD_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_LINE_START, glfwGetKey(win, GLFW_KEY_B) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_TEXT_LINE_END, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
	}
	else {
		nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
		nk_input_key(ctx, NK_KEY_COPY, 0);
		nk_input_key(ctx, NK_KEY_PASTE, 0);
		nk_input_key(ctx, NK_KEY_CUT, 0);
		nk_input_key(ctx, NK_KEY_SHIFT, 0);
	}

	glfwGetCursorPos(win, &x, &y);
	nk_input_motion(ctx, (int)x, (int)y);
	if (ctx->input.mouse.grabbed) {
		glfwSetCursorPos(glfw.win, ctx->input.mouse.prev.x, ctx->input.mouse.prev.y);
		ctx->input.mouse.pos.x = ctx->input.mouse.prev.x;
		ctx->input.mouse.pos.y = ctx->input.mouse.prev.y;
	}

	nk_input_button(ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	nk_input_button(ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
	nk_input_button(ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
	nk_input_scroll(ctx, glfw.scroll);
	nk_input_end(&glfw.ctx);
	glfw.text_len = 0;
	glfw.scroll = 0;
}

NK_API
void nk_glfw3_shutdown(void)
{
	nk_font_atlas_clear(&glfw.atlas);
	nk_free(&glfw.ctx);
	nk_glfw3_device_destroy();
	memset(&glfw, 0, sizeof(glfw));
}


/**************
Empieza la GUI MIA
***************/

struct Gui::GuiData {
	struct nk_context *ctx;
	struct nk_color background;
	struct nk_color DirColor;
};
Gui::Gui() {
	data_ptr_ = new GuiData();
	//GLFWwindow* data;
	//data = (GLFWwindow*)Engine::engine_instance()->window()->window_ptr();
	height_w = Engine::engine_instance()->window()->window_height();
	width_w = Engine::engine_instance()->window()->window_width();
	w_div_4 = width_w / 4;
	h_div_4 = height_w / 4;
	data_ptr_->ctx = nk_glfw3_init((GLFWwindow*)Engine::engine_instance()->window()->window_ptr(), NK_GLFW3_INSTALL_CALLBACKS);

	struct nk_font_atlas *atlas;
	nk_glfw3_font_stash_begin(&atlas);
	nk_glfw3_font_stash_end();
	data_ptr_->DirColor = nk_rgb(127, 127, 127);
	data_ptr_->background = nk_rgb(0, 0, 255);
	pos_ = vec3(0);
	select_theme = 0;

}

void Gui::setTheme(kTema theme_select) {
	switch (theme_select)
	{
	case kTema::kTema_BLACK: set_style(data_ptr_->ctx, THEME_BLACK);
		break;
	case kTema::kTema_WHITE: set_style(data_ptr_->ctx, THEME_WHITE);
		break;
	case kTema::kTema_RED: set_style(data_ptr_->ctx, THEME_RED);
		break;
	case kTema::kTema_BLUE: set_style(data_ptr_->ctx, THEME_BLUE);
		break;
	case kTema::kTema_DARK: set_style(data_ptr_->ctx, THEME_DARK);
		break;
	case kTema::kTema_DEFAULT: nk_style_default(data_ptr_->ctx);
		break;
	}
}
static int show_go = nk_false;
static int show_light = nk_false;
void Gui::gameobject_info() {
	if (show_go == 1) {
		static struct nk_rect s = { 200 , 100, 300, 150 };
		string tmpPop= "GameObject Info " + Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->name();
		if (nk_popup_begin(data_ptr_->ctx, NK_POPUP_STATIC,tmpPop.c_str(), NK_WINDOW_CLOSABLE, s))
		{
			nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);
			vec3 tmpPos = Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->getPosition();
			vec3 tmpRot = Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->getRotation();
			vec3 tmpSca = Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->getScale();
			
			//string  tmp = "Position " + Utils::limit_float_precision(tmpPos);
			//string  tmp2 = "Rotation " + Utils::limit_float_precision(tmpRot);
			//string  tmp3 = "Scale " + Utils::limit_float_precision(tmpSca);
			//nk_label(data_ptr_->ctx, tmp.c_str(), NK_TEXT_LEFT);
			//nk_label(data_ptr_->ctx, tmp2.c_str(), NK_TEXT_LEFT);
			//nk_label(data_ptr_->ctx, tmp3.c_str(), NK_TEXT_LEFT);

			nk_label(data_ptr_->ctx, "Position",NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", -500.0f, &tmpPos.x, 500.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Y:", -500.0f, &tmpPos.y, 500.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Z:", -500.0f, &tmpPos.z, 500.0f, 1.0f, 1.0f);
			nk_label(data_ptr_->ctx, "Rotation", NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", -360.0f, &tmpRot.x, 360.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Y:", -360.0f, &tmpRot.y, 360.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Z:", -360.0f, &tmpRot.z, 360.0f, 1.0f, 1.0f);
			nk_label(data_ptr_->ctx, "Scale", NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", -10.0f, &tmpSca.x, 10.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Y:", -10.0f, &tmpSca.y, 10.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Z:", -10.0f, &tmpSca.z, 10.0f, 1.0f, 1.0f);

			
			
			Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->position(tmpPos);
			Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->rotation(tmpRot);
			Engine::engine_instance()->currentScene()->getEntityManager()->getList()[game_object_selection]->scale(tmpSca);
			nk_popup_end(data_ptr_->ctx);
		}
		else {
			show_go = nk_false;
		}

	}
}

void Gui::gameobject_management() {
	if (nk_tree_push(data_ptr_->ctx, NK_TREE_TAB, "GameObjects", NK_MINIMIZED)) {
		//nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);
		curr_scene_ = Engine::engine_instance()->currentScene();
		for (uint32 i = 0; i < curr_scene_->getEntityManager()->getList().size(); i++) {
			if (nk_button_label(data_ptr_->ctx, curr_scene_->getEntityManager()->getList()[i]->name().c_str())) {
				printf("%s", curr_scene_->getEntityManager()->getList()[i]->name().c_str());
				show_go = !show_go;
				game_object_selection = i;
			}
		}
		nk_tree_pop(data_ptr_->ctx);
	}
}

void Gui::light_info() {
	if (show_light == 1)
	{
		static struct nk_rect s = { 200 , 100, 300, 150 };
		string tmpPop = "Light Info " + Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->get_name();
		if (nk_popup_begin(data_ptr_->ctx, NK_POPUP_STATIC, tmpPop.c_str(), NK_WINDOW_CLOSABLE, s))
		{
			nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);

			vec3 tmpPos = Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->position();
			vec3 color = Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->color();
			vec3 direction = Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->direction();

			//string  tmp = "Position " + Utils::limit_float_precision(tmpPos);
			//string  tmp2 = "Rotation " + Utils::limit_float_precision(tmpRot);
			//string  tmp3 = "Scale " + Utils::limit_float_precision(tmpSca);
			//nk_label(data_ptr_->ctx, tmp.c_str(), NK_TEXT_LEFT);
			//nk_label(data_ptr_->ctx, tmp2.c_str(), NK_TEXT_LEFT);
			//nk_label(data_ptr_->ctx, tmp3.c_str(), NK_TEXT_LEFT);

			nk_label(data_ptr_->ctx, "Position", NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", -500.0f, &tmpPos.x, 500.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Y:", -500.0f, &tmpPos.y, 500.0f, 1.0f, 1.0f);
			nk_property_float(data_ptr_->ctx, "#Z:", -500.0f, &tmpPos.z, 500.0f, 1.0f, 1.0f);
			nk_label(data_ptr_->ctx, "Color", NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", 0.0f, &color.x, 1.0f, 0.1f, 0.1f);
			nk_property_float(data_ptr_->ctx, "#Y:", 0.0f, &color.y, 1.0f, 0.1f, 0.1f);
			nk_property_float(data_ptr_->ctx, "#Z:", 0.0f, &color.z, 1.0f, 0.1f, 0.1f);
			nk_label(data_ptr_->ctx, "Scale", NK_TEXT_LEFT);
			nk_property_float(data_ptr_->ctx, "#X:", -1.0f, &direction.x, 1.0f, 0.2f, 0.2f);
			nk_property_float(data_ptr_->ctx, "#Y:", -1.0f, &direction.y, 1.0f, 0.2f, 0.2f);
			nk_property_float(data_ptr_->ctx, "#Z:", -1.0f, &direction.z, 1.0f, 0.2f, 0.2f);

			Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->set_position(tmpPos);
			Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->set_color(color);
			Engine::engine_instance()->currentScene()->get_lights()[game_light_selection]->set_direction(direction);

			nk_popup_end(data_ptr_->ctx);
		}
		else
		{
			show_light = nk_false;
		}
	}
}


void Gui::light_management(){
	if (nk_tree_push(data_ptr_->ctx, NK_TREE_TAB, "Light", NK_MINIMIZED)) {
		//nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);
		curr_scene_ = Engine::engine_instance()->currentScene();
		for (uint32 i = 0; i < curr_scene_->get_lights().size(); i++) {
			if (nk_button_label(data_ptr_->ctx, curr_scene_->get_lights()[i]->get_name().c_str())) {
				printf("%s", curr_scene_->getEntityManager()->getList()[i]->name().c_str());
				show_light = !show_light;
				game_light_selection = i;
			}
		}
		nk_tree_pop(data_ptr_->ctx);
	}
}


void Gui::sceneManagement() {
	if (nk_tree_push(data_ptr_->ctx, NK_TREE_TAB, "Engine", NK_MINIMIZED)) {
		nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);
		static char field_buffer[64];
		static int field_len;
		nk_edit_string(data_ptr_->ctx, NK_EDIT_FIELD, field_buffer, &field_len, 64, nk_filter_default);
		if (nk_button_label(data_ptr_->ctx, "Load Scene")) {
			printf("\nLoading Scene...");
			Engine::engine_instance()->LoadScene(field_buffer);
		}
		static char field_buffer2[64];
		static int field_len2;
		nk_edit_string(data_ptr_->ctx, NK_EDIT_FIELD, field_buffer2, &field_len2, 64, nk_filter_default);
		if (nk_button_label(data_ptr_->ctx, "Save Scene")) {
			printf("\nSaving Scene....");
			Engine::engine_instance()->SaveScene(string(field_buffer2));
			printf("\nSaving ended.");
		}

		if (nk_button_label(data_ptr_->ctx, "PostrProcess Freichen")) {
			Engine::engine_instance()->currentScene()->freichen *= -1;
		}
		if (nk_button_label(data_ptr_->ctx, "PostrProcess Gradient")) {
			Engine::engine_instance()->currentScene()->gradient *= -1;
		}
		nk_tree_pop(data_ptr_->ctx);
	}

	gameobject_management();
	light_management();
}
#include "Core\OpenGL\aopengl.h"
static int  show_info = nk_false;
void Gui::new_frame() {
	nk_glfw3_new_frame();
	//Gui for Scenes
	//overview(data_ptr_->ctx);

	if (nk_begin(data_ptr_->ctx, Engine::engine_instance()->currentScene()->name_scene().c_str(), nk_rect(0.0f, 0.0f, (float)w_div_4, (float)height_w),
		NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		nk_menubar_begin(data_ptr_->ctx);
		nk_layout_row_begin(data_ptr_->ctx, NK_STATIC, 25, 2);
		nk_layout_row_push(data_ptr_->ctx, 45);
		if (nk_combo_begin_label(data_ptr_->ctx, "DrawMode", nk_vec2(200, 200))) {
			nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
			if (nk_button_label(data_ptr_->ctx,"WireframeMode")) {
				AOpenGL::setWireframMode();
			}
			
			/*for (int i = 0; i < 5; i++) {
				if (nk_button_label(data_ptr_->ctx, string(draw_mode[i]).c_str())) {
					AOpenGL::setDrawMode(i);
				}
			}*/
			nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
			nk_combo_end(data_ptr_->ctx);
		}
		nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
		if (nk_menu_begin_label(data_ptr_->ctx, "Cameras", NK_TEXT_LEFT, nk_vec2(100, 200)))
		{
			for (uint32 size = 0; size < Engine::engine_instance()->currentScene()->camera().size(); size++) {
				nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
				//nk_checkbox_label(data_ptr_->ctx, string("Camera "+std::to_string(size)).c_str(), &check[size]);
				if (nk_button_label(data_ptr_->ctx, string("Camera " + std::to_string(size)).c_str())) {
					Engine::engine_instance()->currentScene()->setCamera(size);
				}
			}
			nk_menu_end(data_ptr_->ctx);
		}
		
		nk_checkbox_label(data_ptr_->ctx, "CameraInfo", &show_info);
		gameobject_info();
		light_info();
		camera_info(&show_info);
		nk_menubar_end(data_ptr_->ctx);
		sceneManagement();
	}
	nk_end(data_ptr_->ctx);
	//Gui for Log
	logWindow();
}


void Gui::camera_info(int * show) {
	if (*show == 1) {
		static struct nk_rect s = { 200 , 100, 300, 100 };
		if (nk_popup_begin(data_ptr_->ctx, NK_POPUP_STATIC, "Camera Info", NK_WINDOW_CLOSABLE, s))
		{
			nk_layout_row_dynamic(data_ptr_->ctx, 20, 1);
			vec3 tmpPos = Engine::engine_instance()->currentScene()->currentCamera()->getPosition();
			string  tmp = "Position " + Utils::limit_float_precision(tmpPos);
			string  tmp2 = "Rotation " + Utils::limit_float_precision(Engine::engine_instance()->currentScene()->currentCamera()->getRotation());
			nk_label(data_ptr_->ctx, tmp.c_str() , NK_TEXT_LEFT);
			nk_label(data_ptr_->ctx,tmp2.c_str() , NK_TEXT_LEFT);
			nk_popup_end(data_ptr_->ctx);
		
			
		}
		else {
			*show = nk_false;
		}
		
	}
}

void Gui::logWindow() {
	if (nk_begin(data_ptr_->ctx, "Log", nk_rect((float)w_div_4, h_div_4*3.0f, (float)width_w - w_div_4, (float)h_div_4), NK_WINDOW_BORDER | NK_WINDOW_TITLE)) {
		nk_layout_row_static(data_ptr_->ctx, 30.0f, 80, 1);
		if (nk_button_label(data_ptr_->ctx, "ClearLog")) {
			Engine::engine_instance()->clear_log();
			Engine::engine_instance()->add_log(string("Clear log"), kLogColors::kColor_Green);
		}

		for (auto& it : Engine::engine_instance()->log_array()) {
			nk_layout_row_dynamic(data_ptr_->ctx, 10.0f, 1);
			string log = it.first.c_str();
			ivec3 tmpColor = it.second;
			nk_label_colored(data_ptr_->ctx, log.c_str(), NK_TEXT_LEFT, nk_rgb(tmpColor.r, tmpColor.g, tmpColor.b));
		}
	}
	nk_end(data_ptr_->ctx);
}

void Gui::draw() {
	nk_glfw3_render(NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void Gui::editor_vec3(string name_, vec3 *object) {
	if (nk_combo_begin_label(data_ptr_->ctx, name_.c_str(), nk_vec2(200, 200))) {
		nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
		nk_property_float(data_ptr_->ctx, "#X:", -1.0f, &object->x, 1.0f, 0.1f, 0.5f);
		nk_property_float(data_ptr_->ctx, "#Y:", -1.0f, &object->y, 1.0f, 0.1f, 0.5f);
		nk_property_float(data_ptr_->ctx, "#Z:", -1.0f, &object->z, 1.0f, 0.1f, 0.5f);
		nk_layout_row_dynamic(data_ptr_->ctx, 25, 1);
		nk_combo_end(data_ptr_->ctx);
	}
	
}

Gui::~Gui() {
	data_ptr_->ctx = nullptr;;
	delete data_ptr_;
	nk_glfw3_shutdown();
}