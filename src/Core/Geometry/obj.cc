#include "Core/Geometry/obj.h"
#include "Core/engine.h"
#define TINYOBJLOADER_IMPLEMENTATION
//#include "external\tiny_obj_loader.h"
#include "external/tinyobjloaderlegacy.h"
using namespace Awesome;

Obj::Obj(string name) {
	name_ = name;
}

Obj::~Obj()
{
}


void Obj::TrueLoad() {
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string err;
	bool ret = tinyobj::LoadObj(shapes, materials, err, name_.c_str());
	//printf("\n%s", name_.c_str());
	for (uint32 i = 0; i < shapes.size(); i++)
	{
		for (uint32 j = 0; j < shapes[i].mesh.positions.size(); j++)
		{
			vertex.push_back(shapes[i].mesh.positions[j]);
			normals.push_back(shapes[i].mesh.normals[j]);
		}

		for (uint32 j = 0; j < shapes[i].mesh.texcoords.size(); j++)
		{
			uv.push_back(shapes[i].mesh.texcoords[j]);
		}
		for (uint32 j = 0; j < shapes[i].mesh.indices.size(); j++)
		{
			index.push_back(shapes[i].mesh.indices[j]);
		}
		std::size_t pos = name_.find_last_of("/");
		name_ = name_.substr(pos + 1);
		//printf("\n%s", name_.c_str());
		numindices = index.size();
	}
	//New tinyOBj
}
void Obj::Start() {
	TrueLoad();
	Create();
}