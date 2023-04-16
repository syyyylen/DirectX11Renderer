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

    ID3D11RenderTargetView* GetRTV();
    ID3D11RenderTargetView** GetRTVLValue();

private:
    IDXGISwapChain* m_swapChain;
    ID3D11RenderTargetView* m_rtv;

    Renderer* m_renderer = nullptr;

private:
    friend class DeviceContext;
};
