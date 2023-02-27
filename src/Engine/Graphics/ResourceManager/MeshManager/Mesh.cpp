#include <locale>
#include <filesystem>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "Engine/Graphics/ResourceManager/MeshManager/Mesh.hpp"
#include "Engine/Graphics/GraphicsEngine.hpp"
#include "Engine/Math/VertexMesh.hpp"

Mesh::Mesh(const wchar_t* fullPath) :
	Resource(fullPath)
{
	tinyobj::attrib_t attribs;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;

	std::string warnings;
	std::string errors;

	auto inputFile = std::filesystem::path(fullPath).string();

	bool res = tinyobj::LoadObj(&attribs, &shapes, &materials, &warnings, &errors, inputFile.c_str());

	if (!errors.empty() || !res || shapes.size() > 1)
	{
		throw std::exception("Mesh not created successfully");
	}

	std::vector<VertexMesh> listVertices;
	std::vector<UINT> listIndices;

	for (size_t s = 0; s < shapes.size(); s++)
	{
		size_t indexOffset = 0;
		listVertices.reserve(shapes[s].mesh.indices.size());
		listIndices.reserve(shapes[s].mesh.indices.size());

		for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
		{
			uint8_t numFaceVertices = shapes[s].mesh.num_face_vertices[f];

			for (uint8_t v = 0; v < numFaceVertices; v++)
			{
				tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];

				tinyobj::real_t vx = attribs.vertices[index.vertex_index * 3 + 0];
				tinyobj::real_t vy = attribs.vertices[index.vertex_index * 3 + 1];
				tinyobj::real_t vz = attribs.vertices[index.vertex_index * 3 + 2];

				tinyobj::real_t tx = attribs.texcoords[index.texcoord_index * 2 + 0];
				tinyobj::real_t ty = attribs.texcoords[index.texcoord_index * 2 + 1];
			
				VertexMesh vertex(Vector3f(vx, vy, vz), Vector2f(tx, ty));
				listVertices.push_back(vertex);
				listIndices.push_back(indexOffset + v);
			}

			indexOffset += numFaceVertices;
		}
	}

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;
	GraphicsEngine::get()->getVMLSByteCodeAndSize(&shaderByteCode, &sizeShader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(&listVertices[0], sizeof(VertexMesh), listVertices.size(), shaderByteCode, sizeShader);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(&listIndices[0], (UINT)listIndices.size());
}

Mesh::~Mesh()
{
}

const VertexBufferPtr& Mesh::getVertexBuffer()
{
	return m_vb;
}

const IndexBufferPtr& Mesh::getIndexBuffer()
{
	return m_ib;
}
