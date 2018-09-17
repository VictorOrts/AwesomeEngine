#ifndef _H_RENDER_TO_TEXTURE_
#define _H_RENDER_TO_TEXTURE_
#include "Core/types.h"
#include "framebuffer.h"
#include "Core/Geometry/plane.h"
#include <memory>
namespace Awesome {

	enum PostProcess {
		kDegradado,
		kWyB,
		Count
	};

	class RenderToTexture : public FrameBuffer
	{
	public:
		RenderToTexture(int16 ,int16);
		~RenderToTexture();
		template <enum PostProcess>
		void setEffect(bool active);
		void begin();
		void end();
		void drawRenderToTexture();
	private:
		void rellenaMap();
		//std::shared_ptr<FrameBuffer> frame_;
		std::shared_ptr<Plane> plane_;
		std::map<PostProcess, int8> valuesPP;
		int8 degradado_;
		int8 blacoynegro_;
		uint32 id_prog_;
	};
}

#endif // !_H_RENDER_TO_TEXTURE_
