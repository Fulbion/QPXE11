#pragma once

#include "Engine/Math/Vector2D.hpp"
#include "Engine/Math/Vector3D.hpp"

class VertexMesh
{
public:
	VertexMesh() :
		position(),
		texcoord()
	{
	}

	VertexMesh(Vector3D<float> position, Vector2D<float> texcoord) :
		position(position),
		texcoord(texcoord)
	{
	}

	VertexMesh(const VertexMesh& vertex) :
		position(vertex.position),
		texcoord(vertex.texcoord)
	{
	}

	~VertexMesh()
	{
	}

public:
	Vector3D<float> position;
	Vector2D<float> texcoord;
};