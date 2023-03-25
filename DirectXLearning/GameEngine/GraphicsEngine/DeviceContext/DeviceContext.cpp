#include "DeviceContext.h"

#include "../SwapChain/SwapChain.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext) : m_deviceContext(deviceContext)
{
    
}

DeviceContext::~DeviceContext()
{
}

bool DeviceContext::ClearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha)
{
    FLOAT clearColor[] = { red, green, blue, alpha };
    m_deviceContext->ClearRenderTargetView(swapChain->m_rtv , clearColor);

    return true;
}

bool DeviceContext::Release()
{
    m_deviceContext->Release();
    delete this;

    return true;
}
