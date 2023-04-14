#include "SwapChain.h"
#include "..\GraphicsEngine.h"

SwapChain::SwapChain()
{
}

SwapChain::~SwapChain()
{
}

bool SwapChain::Init(HWND hwnd, UINT width, UINT height)
{
    ID3D11Device* d3dDevice = GraphicsEngine::Get()->m_d3dDevice;

    DXGI_SWAP_CHAIN_DESC desc;
    ZeroMemory(&desc, sizeof(desc));
    desc.BufferCount = 1;
    desc.BufferDesc.Width = width;
    desc.BufferDesc.Height = height;
    desc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    desc.BufferDesc.RefreshRate.Numerator = 60;
    desc.BufferDesc.RefreshRate.Denominator = 1;
    desc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.OutputWindow = hwnd;
    desc.SampleDesc.Count = 1;
    desc.SampleDesc.Quality = 0;
    desc.Windowed = TRUE;
    
    HRESULT res = GraphicsEngine::Get()->m_dxgiFactory->CreateSwapChain(d3dDevice, &desc, &m_swapChain);

    if(FAILED(res))
    {
        LOG("Failed SwapChain creation");
        return false;
    }


    ID3D11Texture2D* buffer = NULL;
    res = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

    if(FAILED(res))
    {
        LOG("Failed Texture2D buffer creation");
        return false;
    }

    res = d3dDevice->CreateRenderTargetView(buffer, NULL, &m_rtv);
    buffer->Release();

    if(FAILED(res))
    {
        LOG("Failed rtv creation");
        return false;
    }

    LOG("Suceeded SwapChain creation");
    
    return true;
}

bool SwapChain::Release()
{
    if(m_swapChain)
        m_swapChain->Release();

    delete this;
    
    return true;
}

bool SwapChain::Present(bool vsync)
{
    m_swapChain->Present(vsync, NULL);
    
    return true;
}

ID3D11RenderTargetView* SwapChain::GetRTV()
{
    return m_rtv;
}

ID3D11RenderTargetView** SwapChain::GetRTVLValue()
{
    return &m_rtv;
}
