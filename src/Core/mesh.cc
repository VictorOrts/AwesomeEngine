#include "Core/mesh.h"
#include "Core/OpenGL/aopengl.h"
#include "Core/engine.h"
#include "Core\Objects\utils.h"
using namespace Awesome;

Mesh::Mesh() {
	vao_ = 0;
	evo_ = 0;
	numindices = 0;
}

Mesh::~Mesh()
{
	AOpenGL::DeleteBuffers(vao_);
	AOpenGL::DeleteBuffers(evo_);
	/*vertex.clear();
	normals.clear();
	uv.clear();
	index.clear();*/
}

void Mesh::Create() {
	trueCreate();
}

uint32 Mesh::get_evo() {
	return evo_;
}
uint32 Mesh::get_vao() {
	return vao_;
}
uint32 Mesh::get_numindices() {
	return numindices;
}
string Mesh::get_name() {
	return name_;
}

void Mesh::trueCreate() {
	AOpenGL::LoadGeometry(&vao_, &evo_, &numindices, vertex, normals, uv, index);
	//Engine::engine_instance()->addMesh(shared_from_this());
	string prueba = "Vertices -> " + std::to_string(vertex.size()) + " Normales -> " + std::to_string(normals.size()) +
		" UV -> " + std::to_string(uv.size()) + " NIndices -> " + std::to_string(numindices);
	Engine::engine_instance()->add_log(prueba, kLogColors::kColor_Default);
}