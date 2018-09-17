#include "Core\Objects\defaultobjects.h"
#include "Core\Geometry\cube.h"
using namespace Awesome;

DefaultObjects::DefaultObjects() {
	default_cube_ = std::make_shared<Cube>();

	//default_texture_ = std::make_shared<Texture>("blanco.jpg");

	//default_texture_->loadToGPU();
}

DefaultObjects ::~DefaultObjects()
{
}

void DefaultObjects::init() {
	//default_texture_->loadTexture("default.png",Texture::kNearest,Texture::kMirrored_repeat);
	//default_texture_->load();
	//default_texture_->loadToGPU();
	default_cube_->Create();
}
std::shared_ptr<Cube> DefaultObjects::default_cube() {
	return default_cube_;
}
std::shared_ptr<Texture> DefaultObjects::default_texture() {
	return default_texture_;
}