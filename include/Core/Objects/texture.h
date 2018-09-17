#ifndef _H_TEXTURE_
#define _H_TEXTURE_
#include "Core/types.h"
#include <vector>
namespace Awesome {
	class Texture 
	{
	public:
		enum TextureObjet {
			kType_Diffuse = 0,
			kType_Specular,
			kType_Normal
		};
		enum TypeTexture {
			kTypeTexture_RGB = 3,
			kTypeTexture_RGBA
		};
		enum TypeFiltering {
			kNearest = 0,
			kLinear,
			kNearest_mipmap_Nearest,
			kLinear_mipmap_Nearest,
			kNearest_mipmap_Linear,
			kLinear_mipmap_Linear
		};

		enum TypeTexCoord {
			kClamp_to_Edge = 0,
			kClamp_to_Border,
			kMirrored_repeat,
			kRepeat
		};
		enum TypeTextureTexOrCM {
			kType_Texture,
			kType_Cubemap
		};

	public:
		Texture(string name);
		Texture();
		~Texture();
		void loadTexture(string fileName, TypeFiltering filter = kNearest, TypeTexCoord texcoord = kClamp_to_Border);
		void loadTexture(std::vector<string> fileName, TypeFiltering filter = kNearest, TypeTexCoord texcoord = kClamp_to_Border);

		void load();
		void loadToGPU();
		int16 width();
		int16 height();
		uint32 id_texture();
		uint32 texture_unit();
		string name();
		void set_name(string a);
		bool loaded;
	private:
		int16 width_;
		int16 height_;
		int8 type_;
		int8 filtering_;
		int8 tex_coord_;
		uint32 id_texture_;
		uint32 texture_unit_;
		uchar_ptr image_;
		std::vector<uchar_ptr> images;
		string name_;
	};
}

#endif // !_H_TEXTURE_
