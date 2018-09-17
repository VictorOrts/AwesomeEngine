#include "Core/Components/rendercomponent.h"
#include "Core/Commands/draw_command.h"
#include "Core/Commands/program_command.h"
#include "Core/Task/displaytask.h"
#include "Core/engine.h"
#include "Core/Geometry/cube.h"
#include "Core/Geometry/obj.h"
#include "Core/Geometry/plane.h"
using namespace Awesome;

RenderComponent::RenderComponent() {
	type_component_ = kRenderComponent;
	enabled_ = true;
}

RenderComponent::~RenderComponent()
{

}


//Creando mesh dentro
template<>
void RenderComponent::addMesh<Obj>(string a) {
	mesh_ = std::make_shared<Obj>(a);
	mesh_->Start();
}

template<>
void RenderComponent::addMesh<Plane>() {
	mesh_ = std::make_shared<Plane>(Plane::kPlane_Plane);
	mesh_->Start();
}
template<>
void RenderComponent::addMesh<Cube>() {
	mesh_ = std::make_shared<Cube>();
	mesh_->Start();
}



void RenderComponent::addMaterial() {
	material_ = std::make_shared<Material>();
}

std::shared_ptr<Mesh> RenderComponent::mesh_component() const {
	return mesh_;
}

std::shared_ptr<Material> RenderComponent::material_component() const {
	return material_;
}

void RenderComponent::update() {
	//printf("\nCOSITA RC");
}

void RenderComponent::render(mat4 camera_mat, uint32 program) {
	Engine * tmpEngine = Engine::engine_instance();

	mat4 matrix;
	matrix = camera_mat;

	
	//TODO: MEJORAR  ESTO DE CREAR UNA TASK DISPLAYLIST
	std::shared_ptr<ProgramCommand> usepro = std::make_shared<ProgramCommand>(program,material_.get(), game_object()->transform(), matrix);

	std::shared_ptr<DrawCommand> draw = std::make_shared<DrawCommand>(mesh_->get_numindices(),
		mesh_->get_vao(), mesh_->get_evo());
	tmpEngine->get_render()->get_display()->addCommand(usepro);
	tmpEngine->get_render()->get_display()->addCommand(draw);
}