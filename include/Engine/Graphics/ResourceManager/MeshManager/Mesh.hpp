#pragma once

#include "Engine/Graphics/ResourceManager/Resource.hpp"
#include "Engine/Graphics/RenderSystem/VertexBuffer/VertexBuffer.hpp"
#include "Engine/Graphics/RenderSystem/IndexBuffer/IndexBuffer.hpp"

class Mesh :
	public Resource
{
public:
	Mesh(const wchar_t* fullPath);
	~Mesh();

	const VertexBufferPtr& getVertexBuffer();
	const IndexBufferPtr& getIndexBuffer();

private:
	VertexBufferPtr m_vb;
	IndexBufferPtr m_ib;

	friend class DeviceContext;
};
