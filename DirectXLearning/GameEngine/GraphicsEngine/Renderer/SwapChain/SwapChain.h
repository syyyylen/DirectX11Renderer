#pragma once
#include <d3d11.h>*
#include "..\..\..\Logger/Log.h"
#include "..\..\Prerequisites.h"

class SwapChain
{
public:
    SwapChain(Renderer* renderer, HWND hwnd, UINT width, UINT height);
    ~SwapChain();

    bool Present(bool vsync);
    void Resize(unsigned int width, unsigned int height);
    void SetFullscreen(bool fullscreen, unsigned int width, unsigned int height);

    ID3D11RenderTargetView* GetRTV();
    ID3D11RenderTargetView** GetRTVLValue();

private:
    void ReloadBuffers(unsigned int width, unsigned int height);
    
    IDXGISwapChain* m_swapChain = nullptr;
    ID3D11RenderTargetView* m_rtv = nullptr;
    ID3D11DepthStencilView* m_dsv = nullptr;

    Renderer* m_renderer = nullptr;

private:
    friend class DeviceContext;
};
