#ifndef _H_DEFERRED_IMPLEMENTATION_
#define _H_DEFERRED_IMPLEMENTATION_

#include "Core\types.h"
#include "Core\engine.h"
#include "Core\Objects\framebuffer.h"
//Un solo framebuffer con varias texturas
namespace Awesome {
	class Gbuffer
	{
	public:
		enum Gbuffer_types
		{
			kGbuffer_Position=0,
			kGbuffer_Normals,
			kGbuffer_AlbedoSpecular,
			kGbuffer_Size
		};
		Gbuffer();
		~Gbuffer();
		void InitDeferred();
		uint32 framebuffer();
		std::map<uint32, string> textures();
		void bindFramebuffer();
		void unbindFramebuffer();
		
	private:
		std::vector<std::shared_ptr<FrameBuffer>> gbufferList_;
		uint32 framebuffer_id_;
		uint32 renderbuffer_id_;
		std::map<uint32, string> textures_;

	};

}

#endif // !_H_DEFERRED_IMPLEMENTATION_
