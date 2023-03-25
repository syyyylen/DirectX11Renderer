#pragma once
#include <d3d11.h>

class SwapChain;

class DeviceContext
{
public:
    DeviceContext(ID3D11DeviceContext* deviceContext);
    ~DeviceContext();

    bool ClearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha);
    
    bool Release();

private:
    ID3D11DeviceContext* m_deviceContext;
};
