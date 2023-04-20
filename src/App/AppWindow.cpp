#include "App/AppWindow.hpp"
#include <Windows.h>

struct Vertex
{
	float3 position;
	float2 texcoord;
};

__declspec(align(16))
struct Constant
{
	float4x4 m_world;
	float4x4 m_view;
	float4x4 m_projection;
	float4 m_lightDirection;
	float4 m_cameraPosition;
};

AppWindow::AppWindow()
{
	GraphicsEngine::create();
	InputSystem::create();
}

AppWindow::~AppWindow()
{
	InputSystem::release();
	GraphicsEngine::release();
}

void AppWindow::render()
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->clearRenderTargetColor(this->m_swapChain, 0.1f, 0.1f, 0.1f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setViewportSize(rc.right - rc.left, rc.bottom - rc.top);

	update();

	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(CullType::Back);
	drawMesh(m_testModel, m_vs, m_ps, m_cb, m_testTexture);
	GraphicsEngine::get()->getRenderSystem()->setRasterizerState(CullType::Front);
	drawMesh(m_skyMesh, m_vs, m_skyShader, m_skyConstantBuffer, m_skyTexture);

	m_swapChain->present(true);

	m_oldDelta = m_newDelta;
	m_newDelta = GetTickCount64();

	m_deltaTime = (m_oldDelta) ? ((m_newDelta - m_oldDelta) / 1000.0f) : 0;
}

void AppWindow::update()
{
	updateCamera();
	updateSkyBox();
	updateModel();
}

void AppWindow::updateModel()
{
	Constant cc;
	
	float4x4 lightRotationMatrix;
	lightRotationMatrix.setIdentity();
	lightRotationMatrix.rotateY(m_lightRotationY);
	m_lightRotationY += degToRad(45) * m_deltaTime;

	cc.m_world.setIdentity();
	cc.m_view = m_viewCamera;
	cc.m_projection = m_projectionCamera;
	cc.m_cameraPosition = m_worldCamera.getTranslation();
	cc.m_lightDirection = lightRotationMatrix.getZDirection();

	m_cb->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateSkyBox()
{
	Constant cc;

	cc.m_world.setIdentity();
	cc.m_world.scale(Vector3f(100.0f, 100.0f, 100.0f));
	cc.m_world.translate(m_worldCamera.getTranslation());
	cc.m_view = m_viewCamera;
	cc.m_projection = m_projectionCamera;

	m_skyConstantBuffer->update(GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext(), &cc);
}

void AppWindow::updateCamera()
{
	float4x4 worldCamera, temp;
	worldCamera.setIdentity();

	temp.setIdentity();
	temp.rotateX(m_rotationX);
	worldCamera *= temp;

	temp.setIdentity();
	temp.rotateY(m_rotationY);
	worldCamera *= temp;

	Vector3f newPosition = m_worldCamera.getTranslation() + worldCamera.getZDirection() * (m_forward * 0.025f);
	newPosition = newPosition + worldCamera.getXDirection() * (m_rightward * 0.025f);

	worldCamera.translate(newPosition);
	m_worldCamera = worldCamera;
	worldCamera.inverse();

	m_viewCamera = worldCamera;

	// cc.m_projection.setOrthoLH
	// (
	// 	(this->getClientWindowRect().right - this->getClientWindowRect().left) / 300.0f,
	// 	(this->getClientWindowRect().bottom - this->getClientWindowRect().top) / 300.0f,
	// 	-4.0f,
	// 	4.0f
	// );

	UINT width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	UINT height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_projectionCamera.setPrespectiveFovLH(degToRad(90), ((float)width / (float)height), 0.1f, 100.0f);
}

void AppWindow::drawMesh(const MeshPtr& mesh, const VertexShaderPtr& vs, const PixelShaderPtr& ps, const ConstantBufferPtr& cb, const TexturePtr& texture)
{
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(vs, cb);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setConstantBuffer(ps, cb);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexShader(vs);
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setPixelShader(ps);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setTexture(ps, texture);

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setVertexBuffer(mesh->getVertexBuffer());
	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->setIndexBuffer(mesh->getIndexBuffer());

	GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext()->drawIndexedTriangleList(mesh->getIndexBuffer()->getSizeIndexList(), 0, 0);
}

void AppWindow::run()
{
	while (isRunning())
	{

	}
}

void AppWindow::onCreate()
{
	Window::onCreate();

	InputSystem::get()->addListener(this);
	InputSystem::get()->showCursor(false);

	m_gameState = true;

	m_testTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"resources/images/textures/test.png");
	m_testModel = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"resources/models/torus.obj");

	m_skyTexture = GraphicsEngine::get()->getTextureManager()->createTextureFromFile(L"resources/images/textures/sky.jpg");
	m_skyMesh = GraphicsEngine::get()->getMeshManager()->createMeshFromFile(L"resources/models/sphere.obj");

	RECT rc = this->getClientWindowRect();
	m_swapChain = GraphicsEngine::get()->getRenderSystem()->createSwapChain(this->m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	m_worldCamera.translate(Vector3f(0, 0, -1));

	float3 positionList[] =
	{
		{ float3(-0.5f, -0.5f, -0.5f) },
		{ float3(-0.5f, 0.5f, -0.5f) },
		{ float3(0.5f, 0.5f, -0.5f) },
		{ float3(0.5f, -0.5f, -0.5f) },

		{ float3(0.5f,-0.5f,0.5f) },
		{ float3(0.5f,0.5f,0.5f) },
		{ float3(-0.5f,0.5f,0.5f)},
		{ float3(-0.5f,-0.5f,0.5f) }
	};

	float2 texcoordList[] =
	{
		{ float2(0.0f, 0.0f) },
		{ float2(0.0f, 1.0f) },
		{ float2(1.0f, 0.0f) },
		{ float2(1.0f, 1.0f) }
	};

	Vertex vertexList[] =
	{
		{ positionList[0], texcoordList[1] },
		{ positionList[1], texcoordList[0] },
		{ positionList[2], texcoordList[2] },
		{ positionList[3], texcoordList[3] },
		
		{ positionList[4], texcoordList[1] },
		{ positionList[5], texcoordList[0] },
		{ positionList[6], texcoordList[2] },
		{ positionList[7], texcoordList[3] },
		
		{ positionList[1], texcoordList[1] },
		{ positionList[6], texcoordList[0] },
		{ positionList[5], texcoordList[2] },
		{ positionList[2], texcoordList[3] },
		
		{ positionList[7], texcoordList[1] },
		{ positionList[0], texcoordList[0] },
		{ positionList[3], texcoordList[2] },
		{ positionList[4], texcoordList[3] },
		 
		{ positionList[3], texcoordList[1] },
		{ positionList[2], texcoordList[0] },
		{ positionList[5], texcoordList[2] },
		{ positionList[4], texcoordList[3] },
		
		{ positionList[7], texcoordList[1] },
		{ positionList[6], texcoordList[0] },
		{ positionList[1], texcoordList[2] },
		{ positionList[0], texcoordList[3] }
	};

	UINT sizeList = ARRAYSIZE(vertexList);

	UINT indexList[] =
	{
		0, 1, 2,
		2, 3, 0,
		
		4, 5, 6,
		6, 7, 4,
		
		8, 9, 10,
		10, 11, 8,
		
		12, 13, 14,
		14, 15, 12,
		
		16, 17, 18,
		18, 19, 16,
		
		20, 21, 22,
		22, 23, 20
	};

	UINT sizeIndexList = ARRAYSIZE(indexList);
	m_ib = GraphicsEngine::get()->getRenderSystem()->createIndexBuffer(indexList, sizeIndexList);

	void* shaderByteCode = nullptr;
	size_t sizeShader = 0;

	GraphicsEngine::get()->getRenderSystem()->compileVertexShader(L"resources/shaders/VertexShader.hlsl", "vsmain", &shaderByteCode, &sizeShader);

	m_vs = GraphicsEngine::get()->getRenderSystem()->createVertexShader(shaderByteCode, sizeShader);
	m_vb = GraphicsEngine::get()->getRenderSystem()->createVertexBuffer(vertexList, sizeof(Vertex), sizeList, shaderByteCode, (UINT)sizeShader);

	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"resources/shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_ps = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	GraphicsEngine::get()->getRenderSystem()->compilePixelShader(L"resources/shaders/SkyBoxShader.hlsl", "psmain", &shaderByteCode, &sizeShader);
	m_skyShader = GraphicsEngine::get()->getRenderSystem()->createPixelShader(shaderByteCode, sizeShader);
	GraphicsEngine::get()->getRenderSystem()->releaseCompiledShader();

	Constant cc;

	m_cb = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constant));
	m_skyConstantBuffer = GraphicsEngine::get()->getRenderSystem()->createConstantBuffer(&cc, sizeof(Constant));
}

void AppWindow::onUpdate()
{
	Window::onUpdate();

	InputSystem::get()->update();

	this->render();
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	// delete m_vb;
	// delete m_ib;
	// delete m_cb;
	// delete m_swapChain;
	// delete m_vs;
	// delete m_ps;
	m_swapChain->setFullscreen(false, 1, 1);
}

void AppWindow::onFocus()
{
	InputSystem::get()->addListener(this);
}

void AppWindow::onKillFocus()
{
	InputSystem::get()->removeListener(this);
}

void AppWindow::onSize()
{
	RECT rc = this->getClientWindowRect();
	m_swapChain->resize(rc.right, rc.bottom);
	this->onUpdate();
}

void AppWindow::onKeyPressed(int key)
{
	if (key == 'Z')
	{
		m_forward = 1.0f;
	}

	if (key == 'S')
	{
		m_forward = -1.0f;
	}

	if (key == 'Q')
	{
		m_rightward = -1.0f;
	}

	if (key == 'D')
	{
		m_rightward = 1.0f;
	}
}

void AppWindow::onKeyReleased(int key)
{
	m_forward = 0.0f;
	m_rightward = 0.0f;

	if (key == VK_ESCAPE)
	{
		m_gameState = m_gameState ? false : true;
		InputSystem::get()->showCursor(!m_gameState);
	}

	if (key == VK_F11)
	{
		m_fullscreen = m_fullscreen ? false : true;

		RECT screenSize = this->getScreenSize();
		m_swapChain->setFullscreen(m_fullscreen, screenSize.right, screenSize.bottom);
	}
}

void AppWindow::onMouseMove(const Vector2i& mousePosition)
{
	if (!m_gameState) return;

	UINT width = this->getClientWindowRect().right - this->getClientWindowRect().left;
	UINT height = this->getClientWindowRect().bottom - this->getClientWindowRect().top;

	m_rotationX += (mousePosition.y - (height / 2.0f)) * m_deltaTime * 0.5f;
	m_rotationY += (mousePosition.x - (width / 2.0f)) * m_deltaTime * 0.5f;

	InputSystem::get()->setCursorPosition(Vector2i(width / 2.0f, height / 2.0f));
}

void AppWindow::onMouseLeftButtonPressed(const Vector2i& MousePosition)
{
}

void AppWindow::onMouseLeftButtonReleased(const Vector2i& MousePosition)
{
}

void AppWindow::onMouseMiddleButtonPressed(const Vector2i& MousePosition)
{
}

void AppWindow::onMouseMiddleButtonReleased(const Vector2i& MousePosition)
{
}

void AppWindow::onMouseRightButtonPressed(const Vector2i& MousePosition)
{
}

void AppWindow::onMouseRightButtonReleased(const Vector2i& MousePosition)
{
}

