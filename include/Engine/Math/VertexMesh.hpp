#pragma once

#include "Engine/Math/Vector2D.hpp"
#include "Engine/Math/Vector3D.hpp"

class VertexMesh
{
public:
	VertexMesh() :
		position(),
		texcoord(),
		normal()
	{
	}

	VertexMesh(Vector3D<float> position, Vector2D<float> texcoord, Vector3D<float> normal) :
		position(position),
		texcoord(texcoord),
		normal(normal)
	{
	}

	VertexMesh(const VertexMesh& vertex) :
		position(vertex.position),
		texcoord(vertex.texcoord),
		normal(vertex.normal)
	{
	}

	~VertexMesh()
	{
	}

public:
	Vector3D<float> position;
	Vector2D<float> texcoord;
	Vector3D<float> normal;
};