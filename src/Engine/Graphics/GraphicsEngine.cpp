#include <d3dcompiler.h>

#include "Engine/Graphics/GraphicsEngine.hpp"

GraphicsEngine* GraphicsEngine::m_engine = nullptr;

GraphicsEngine::GraphicsEngine()
{
	m_renderSystem = new RenderSystem();
	m_textureManager = new TextureManager();
	m_meshManager = new MeshManager();

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;
	m_renderSystem->compileVertexShader(L"resources/shaders/VertexMeshLayoutShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);
	memcpy(m_meshLayoutByteCode, shaderByteCode, sizeShader);
	m_meshLayoutSize = sizeShader;
	m_renderSystem->releaseCompiledShader();

}

GraphicsEngine::~GraphicsEngine()
{
	GraphicsEngine::m_engine = nullptr;
	delete m_meshManager;
	delete m_textureManager;
	delete m_renderSystem;
}

void GraphicsEngine::create()
{
	if (GraphicsEngine::m_engine) throw std::exception("Graphics Engine already created");
	GraphicsEngine::m_engine = new GraphicsEngine();
}

void GraphicsEngine::release()
{
	if (!GraphicsEngine::m_engine) return;
	delete GraphicsEngine::m_engine;
}

GraphicsEngine* GraphicsEngine::get()
{
	return m_engine;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return m_renderSystem;
}

TextureManager* GraphicsEngine::getTextureManager()
{
	return m_textureManager;
}

MeshManager* GraphicsEngine::getMeshManager()
{
	return m_meshManager;
}

void GraphicsEngine::getVMLSByteCodeAndSize(void** byteCode, size_t* size)
{
	*byteCode = m_meshLayoutByteCode;
	*size = m_meshLayoutSize;
}
