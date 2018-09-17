#ifndef _H_TEXTURE_TASK_
#define _H_TEXTURE_TASK_

#include "task.h"
#include "Core/Objects/texture.h"
namespace Awesome {
	class TextureTask : public  Task {
	public:
		TextureTask(Texture *  tmp);
		~TextureTask();
		void execute();
	private:

		Texture * tmpTexture_;
	};
}

#endif // !_H_TEXTURE_TASK_
