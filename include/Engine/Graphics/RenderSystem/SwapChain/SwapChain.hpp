#pragma once

#include <d3d11.h>

#include "Engine/Prerequisites.hpp"

class SwapChain
{
public:
	SwapChain(HWND hwnd, UINT width, UINT height, RenderSystem* system);
	~SwapChain();

	void resize(UINT width, UINT height);
	void setFullscreen(bool fullscreen, UINT width, UINT height);
	bool present(bool vSync);

private:
	void reloadBuffers(UINT width, UINT height);

	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11RenderTargetView* m_rtv = nullptr;
	ID3D11DepthStencilView* m_dsv = nullptr;
	RenderSystem* m_system = nullptr;

	friend class DeviceContext;
};
