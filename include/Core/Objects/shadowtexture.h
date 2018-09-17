#ifndef SHADOWTEXTURE_H
#define SHADOWTEXTURE_H
#include "Core/types.h"
#include "framebuffer.h"
namespace Awesome {
	class ShadowTexture :public FrameBuffer
	{
	public:
		ShadowTexture(int16 , int16);
		~ShadowTexture();
		void begin();
		void end();
	private:
	};
} // Awesome

#endif // SHADOWTEXTURE_H
