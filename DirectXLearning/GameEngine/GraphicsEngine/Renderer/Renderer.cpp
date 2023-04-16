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
#include "ConstantBuffer/ConstantBuffer.h"

Renderer::Renderer()
{
}

Renderer::~Renderer()
{
}

bool Renderer::Init()
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
        return false;
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

    m_immDeviceContext = new DeviceContext(m_immContext, this);
    
    m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
    m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
    m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

    LOG("Graphics Engine Initialized");

    return true;
}

bool Renderer::Release()
{
    if(m_d3dDevice)
        m_d3dDevice->Release();

    if(m_dxgiAdapter)
        m_dxgiAdapter->Release();

    if(m_dxgiFactory)
        m_dxgiFactory->Release();

    delete m_immDeviceContext;
    
    if(m_d3dDevice)
        m_d3dDevice->Release();

    LOG("Graphics Engine Released");
    
    return true;
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

SwapChain* Renderer::CreateSwapChain(HWND hwnd, UINT width, UINT height)
{
    SwapChain* swapChain = nullptr;
    try
    {
        swapChain = new SwapChain(this, hwnd, width, height);
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

DeviceContext* Renderer::GetImmediateDeviceContext()
{
    return m_immDeviceContext;
}

VertexBuffer* Renderer::CreateVertexBuffer(void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize)
{
    VertexBuffer* vertexBuffer = nullptr;
    try
    {
        vertexBuffer = new VertexBuffer(this, listVertices, vertexSize, listSize, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return vertexBuffer;
}

IndexBuffer* Renderer::CreateIndexBuffer(void* listIndices, UINT listSize)
{
    IndexBuffer* indexBuffer = nullptr;
    try
    {
        indexBuffer = new IndexBuffer(this, listIndices, listSize);
    }
    catch (...) {}
    return indexBuffer;
}

ConstantBuffer* Renderer::CreateConstantBuffer(void* buffer, UINT bufferSize)
{
    ConstantBuffer* constantBuffer = nullptr;
    try
    {
        constantBuffer = new ConstantBuffer(this, buffer, bufferSize);
    }
    catch (...) {}
    return constantBuffer;
}

VertexShader* Renderer::CreateVertexShader(const void* shaderByteCode, size_t shaderByteSize)
{
    VertexShader* vertexShader = nullptr;
    try
    {
        vertexShader = new VertexShader(this, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return vertexShader;
}

PixelShader* Renderer::CreatePixelShader(const void* shaderByteCode, size_t shaderByteSize)
{
    PixelShader* pixelShader = nullptr;
    try
    {
        pixelShader = new PixelShader(this, shaderByteCode, shaderByteSize);
    }
    catch (...) {}
    return pixelShader;
}
