#include "Core\Task\texturetask.h"

using namespace Awesome;

TextureTask::TextureTask(Texture *  tmp) {
	tmpTexture_ = tmp;
}

TextureTask::~TextureTask() {
}
void TextureTask::execute() {
	printf("\nTASK::TEXTURE %s", tmpTexture_->name().c_str());
	
	tmpTexture_->loadTexture(tmpTexture_->name());
	//tmpTexture_->load();
	//tmpTexture_->loadToGPU();
}