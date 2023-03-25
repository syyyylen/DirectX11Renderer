#pragma once
#include <d3d11.h>*
#include "..\..\Logger/Log.h"

class DeviceContext;

class SwapChain
{
public:
    SwapChain();
    ~SwapChain();

    bool Init(HWND hwnd, UINT width, UINT height);
    bool Release();

    bool Present(bool vsync);

private:
    IDXGISwapChain* m_swapChain;
    ID3D11RenderTargetView* m_rtv;

private:
    friend class DeviceContext;
};
