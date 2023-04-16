#include "Renderer.h"
#include"../../Logger/Log.h"
#include "SwapChain/SwapChain.h"
#include "DeviceContext/DeviceContext.h"
#include "VertexBuffer/VertexBuffer.h"
#include "IndexBuffer/IndexBuffer.h"
#include "VertexShader/VertexShader.h"
#include "PixelShader/PixelShader.h"
#include <d3dcompiler.h>
#include <system_error>
#include <exception>
#include "ConstantBuffer/ConstantBuffer.h"

Renderer::Renderer()
{
    D3D_DRIVER_TYPE driver_types[]=
    {
        D3D_DRIVER_TYPE_HARDWARE,
        D3D_DRIVER_TYPE_WARP,
        D3D_DRIVER_TYPE_REFERENCE
    };
    UINT num_driver_types = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[]=
    {
        D3D_FEATURE_LEVEL_11_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);

    HRESULT res = 0;

    for (UINT driver_type_index = 0; driver_type_index < num_driver_types;)
    {
        res =D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels,
            num_feature_levels, D3D11_SDK_VERSION, &m_d3dDevice, &m_featureLevel, &m_immContext);
        if (SUCCEEDED(res))
            break;
        ++driver_type_index;
    }
    if (FAILED(res))
    {
        LOG("Renderer : Create D3D11 Device failed");
        throw std::exception("Renderer : Create D3D11 Device failed");
    }

    // Changing rasterizer properties & state 
    D3D11_RASTERIZER_DESC rasterizerDesc;
    ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
    rasterizerDesc.FillMode = D3D11_FILL_SOLID;
    rasterizerDesc.CullMode = D3D11_CULL_NONE; // Disable culling of face with counterclockwise vertices indexes
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.SlopeScaledDepthBias = 0.0f;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = true;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.AntialiasedLineEnable = false;

    ID3D11RasterizerState* rasterizerState;
    m_d3dDevice->CreateRasterizerState(&rasterizerDesc, &rasterizerState);
    m_immContext->RSSetState(rasterizerState);

    m_immDeviceContext = std::make_shared<DeviceContext>(m_immContext, this);
    
    m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
    m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
    m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

    LOG("Renderer Initialized");
}

Renderer::~Renderer()
{
    if(m_d3dDevice)
        m_d3dDevice->Release();

    if(m_dxgiAdapter)
        m_dxgiAdapter->Release();

    if(m_dxgiFactory)
        m_dxgiFactory->Release();

    if(m_d3dDevice)
        m_d3dDevice->Release();

    LOG("Renderer Destroyed");
}

bool Renderer::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
    ID3DBlob* errorBlob = nullptr;
    HRESULT res;
    
    res = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "vs_5_0", 0, 0, &m_blob, &errorBlob);

    if(!SUCCEEDED(res))
    {
        LOG("Compile Vertex Shader from file failed ! HRESULT : ");

        // HRESULT to msg, super usefull
        std::string errorMsg = std::system_category().message(res);
        LOG(errorMsg);

        // blob to string with an eventual error message 
        if (errorBlob) 
        {
            std::string errorMessage(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
            LOG(errorMessage);
            errorBlob->Release();
        }

        return false;
    }

    *shaderByteCode = m_blob->GetBufferPointer();
    *byteCodeSize = m_blob->GetBufferSize();

    LOG("Vertex Shader Compilation succeeded !");

    return true;
}

bool Renderer::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
{
    ID3DBlob* errorBlob = nullptr;
    HRESULT res;
    
    res = D3DCompileFromFile(fileName, nullptr, nullptr, entryPointName, "ps_5_0", 0, 0, &m_blob, &errorBlob);

    if(!SUCCEEDED(res))
    {
        LOG("Compile Pixel Shader from file failed ! HRESULT : ");

        // HRESULT to msg, super usefull
        std::string errorMsg = std::system_category().message(res);
        LOG(errorMsg);

        // blob to string with an eventual error message 
        if (errorBlob) 
        {
            std::string errorMessage(static_cast<const char*>(errorBlob->GetBufferPointer()), errorBlob->GetBufferSize());
            LOG(errorMessage);
            errorBlob->Release();
        }

        return false;
    }

    *shaderByteCode = m_blob->GetBufferPointer();
    *byteCodeSize = m_blob->GetBufferSize();

    LOG("Pixel Shader Compilation succeeded !");

    return true;
}

void Renderer::ReleaseCompiledShader()
{
    if(m_blob)
        m_blob->Release();
}

std::shared_ptr<SwapChain> Renderer::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
    std::shared_ptr<SwapChain> swapChain = nullptr;
    try
    {
        swapChain = std::make_shared<SwapChain>(this, hwnd, width, height);
    }
    catch (...) {}
    return swapChain;
}

ID3D11Device* Renderer::GetDevice()
{
    return m_d3dDevice;
}

ID3D11DeviceContext* Renderer::GetDeviceContext()
{
    return m_immContext;
}

std::shared_ptr<DeviceContext> Renderer::GetImmediateDeviceContext()
{
    return m_immDeviceContext;
}

std::shared_ptr<VertexBuffer> Renderer::CreateVertexBuffer(void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize)
{
    std::shared_ptr<VertexBuffer> vertexBuffer = nullptr;
    try
    {
        vertexBuffer =  std::make_shared<VertexBuffer>(this, listVertices, vertexSize, listSize, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return vertexBuffer;
}

std::shared_ptr<IndexBuffer> Renderer::CreateIndexBuffer(void* listIndices, UINT listSize)
{
    std::shared_ptr<IndexBuffer> indexBuffer = nullptr;
    try
    {
        indexBuffer = std::make_shared<IndexBuffer>(this, listIndices, listSize);
    }
    catch (...) {}
    return indexBuffer;
}

std::shared_ptr<ConstantBuffer> Renderer::CreateConstantBuffer(void* buffer, UINT bufferSize)
{
    std::shared_ptr<ConstantBuffer> constantBuffer = nullptr;
    try
    {
        constantBuffer = std::make_shared<ConstantBuffer>(this, buffer, bufferSize);
    }
    catch (...) {}
    return constantBuffer;
}

std::shared_ptr<VertexShader> Renderer::CreateVertexShader(const void* shaderByteCode, size_t shaderByteSize)
{
    std::shared_ptr<VertexShader> vertexShader = nullptr;
    try
    {
        vertexShader = std::make_shared<VertexShader>(this, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return vertexShader;
}

std::shared_ptr<PixelShader> Renderer::CreatePixelShader(const void* shaderByteCode, size_t shaderByteSize)
{
    std::shared_ptr<PixelShader> pixelShader = nullptr;
    try
    {
        pixelShader = std::make_shared<PixelShader>(this, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return pixelShader;
}
