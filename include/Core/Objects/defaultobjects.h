#ifndef _H_DEFAULT_OBJECT_
#define _H_DEFAULT_OBJECT_

#include "Core/types.h"
#include "texture.h"

#include <memory>
namespace Awesome {
	class Cube;
	class DefaultObjects {
	public:
		DefaultObjects();
		~DefaultObjects();
		std::shared_ptr<Cube> default_cube();
		std::shared_ptr<Texture> default_texture();
		void init();
		std::shared_ptr<Cube> default_cube_;
		std::shared_ptr<Texture> default_texture_;
	private:
	};
}

#endif
