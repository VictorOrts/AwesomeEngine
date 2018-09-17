#include "Core/Geometry/plane.h"

using namespace Awesome;

Plane::Plane(TypePlane a)
{
	if (a == kPlane_RTT)
	{
		float vertexData[12]{
			-1.0, -1.0, 1.0,
			1.0, -1.0, 1.0,
			1.0, 1.0, 1.0,
			-1.0, 1.0, 1.0
		};

		float uvData[8]{
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};
		uint32 indices[6] = {
			0, 1, 2, 0, 2, 3, // front
		};

		for (int i = 0; i < 12; i++)
		{
			vertex.push_back(vertexData[i]);
		}
		for (int i = 0; i < 8; i++)
		{
			uv.push_back(uvData[i]);
		}
		for (int i = 0; i < 6; i++)
		{
			index.push_back(indices[i]);
		}
		numindices = index.size();
	}
	else
	{
		float vertexData[12]{
			-1.0, 0.0, -1.0,
			1.0, 0.0, -1.0,
			1.0, 0.0, 1.0,
			-1.0, 0.0, 1.0
		};
		float normalData[12]{
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
		};
		float uvData[8]{
			0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		};
		uint32 indices[6] = {
			0, 1, 2, 0, 2, 3, // front
		};

		for (uint16 i = 0; i < 12; i++)
		{
			vertex.push_back(vertexData[i]);
			normals.push_back(normalData[i]);
		}
		for (uint16 i = 0; i < 8; i++)
		{
			uv.push_back(uvData[i]);
		}
		for (uint16 i = 0; i < 6; i++)
		{
			index.push_back(indices[i]);
		}
		numindices = index.size();
	}
}

Plane::~Plane()
{
}

void Plane::Start()
{
	Create();
}
