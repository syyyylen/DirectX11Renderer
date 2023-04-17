#include "SwapChain.h"

#include "../Renderer.h"
#include "..\..\GraphicsEngine.h"
#include <exception>

SwapChain::SwapChain(Renderer* renderer, HWND hwnd, UINT width, UINT height) : m_renderer(renderer)
{
    ID3D11Device* d3dDevice = m_renderer->m_d3dDevice;

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
    desc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    desc.Windowed = TRUE;
    
    HRESULT res = m_renderer->m_dxgiFactory->CreateSwapChain(d3dDevice, &desc, &m_swapChain);

    if(FAILED(res))
    {
        LOG("Failed SwapChain creation");
        throw std::exception("Failed SwapChain creation");
    }

    ReloadBuffers(width, height);
}

SwapChain::~SwapChain()
{
    if(m_rtv)
        m_rtv->Release();
    
    if(m_swapChain)
        m_swapChain->Release();
}

bool SwapChain::Present(bool vsync)
{
    m_swapChain->Present(vsync, NULL);
    
    return true;
}

void SwapChain::Resize(unsigned width, unsigned height)
{
    if(m_rtv)
        m_rtv->Release();
    
    if(m_dsv)
        m_dsv->Release();
    
    m_swapChain->ResizeBuffers(1, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    ReloadBuffers(width, height);
}

void SwapChain::SetFullscreen(bool fullscreen, unsigned width, unsigned height)
{
    Resize(width, height);
    m_swapChain->SetFullscreenState(fullscreen, nullptr);
}

ID3D11RenderTargetView* SwapChain::GetRTV()
{
    return m_rtv;
}

ID3D11RenderTargetView** SwapChain::GetRTVLValue()
{
    return &m_rtv;
}

void SwapChain::ReloadBuffers(unsigned width, unsigned height)
{
    ID3D11Device* d3dDevice = m_renderer->m_d3dDevice;
    
    ID3D11Texture2D* buffer = NULL;
    HRESULT res = m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&buffer);

    if(FAILED(res))
    {
        LOG("Failed Texture2D buffer creation");
        throw std::exception("Failed Texture2D buffer creation");
    }

    res = d3dDevice->CreateRenderTargetView(buffer, NULL, &m_rtv);
    buffer->Release();

    if(FAILED(res))
    {
        LOG("Failed rtv creation");
        throw std::exception("Failed rtv creation");
    }

    // Depth buffer 
    D3D11_TEXTURE2D_DESC tex_desc = {};
    tex_desc.Width = width;
    tex_desc.Height = height;
    tex_desc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    tex_desc.Usage = D3D11_USAGE_DEFAULT;
    tex_desc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    tex_desc.MipLevels = 1;
    tex_desc.SampleDesc.Count = 1;
    tex_desc.SampleDesc.Quality = 0;
    tex_desc.MiscFlags = 0;
    tex_desc.ArraySize = 1;
    tex_desc.CPUAccessFlags = 0;

    res = d3dDevice->CreateTexture2D(&tex_desc, nullptr, &buffer);
    if(FAILED(res))
    {
        LOG("Failed depth buffer creation");
        throw std::exception("Failed depth buffer creation");
    }

    res = d3dDevice->CreateDepthStencilView(buffer, NULL, &m_dsv);
    if(FAILED(res))
    {
        LOG("Failed depth buffer creation");
        throw std::exception("Failed depth buffer creation");
    }

    buffer->Release();
}
