#pragma once

#include <d3d11.h>

#include "Engine/Prerequisites.hpp"
#include "Engine/Graphics/RenderSystem/RenderSystem.hpp"
#include "Engine/Graphics/ResourceManager/TextureManager/TextureManager.hpp"
#include "Engine/Graphics/ResourceManager/MeshManager/MeshManager.hpp"

class GraphicsEngine
{
public:
	RenderSystem* getRenderSystem();
	TextureManager* getTextureManager();
	MeshManager* getMeshManager();

	void getVMLSByteCodeAndSize(void** byteCode, size_t* size);

	static GraphicsEngine* get();

	static void create();
	static void release();

private:
	GraphicsEngine();
	~GraphicsEngine();

	RenderSystem* m_renderSystem = nullptr;
	TextureManager* m_textureManager = nullptr;
	MeshManager* m_meshManager = nullptr;
	static GraphicsEngine* m_engine;

	uint8_t m_meshLayoutByteCode[1024];
	size_t m_meshLayoutSize = 0;
};
