#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "SwapChain/SwapChain.h"
#include "DeviceContext/DeviceContext.h"
#include "VertexBuffer/VertexBuffer.h"
#include "IndexBuffer/IndexBuffer.h"
#include "VertexShader/VertexShader.h"
#include "PixelShader/PixelShader.h"
#include <d3dcompiler.h>
#include <system_error>
#include "ConstantBuffer/ConstantBuffer.h"

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::Init()
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

    m_immDeviceContext = new DeviceContext(m_immContext);
    
    m_d3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&m_dxgiDevice);
    m_dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&m_dxgiAdapter);
    m_dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&m_dxgiFactory);

    LOG("Graphics Engine Initialized");

    return true;
}

bool GraphicsEngine::Release()
{
    if(m_d3dDevice)
        m_d3dDevice->Release();

    if(m_dxgiAdapter)
        m_dxgiAdapter->Release();

    if(m_dxgiFactory)
        m_dxgiFactory->Release();
    
    if(m_immDeviceContext)
        m_immDeviceContext->Release();
    
    if(m_d3dDevice)
        m_d3dDevice->Release();

    LOG("Graphics Engine Released");
    
    return true;
}

bool GraphicsEngine::CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
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

bool GraphicsEngine::CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize)
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

void GraphicsEngine::ReleaseCompiledShader()
{
    if(m_blob)
        m_blob->Release();
}

GraphicsEngine* GraphicsEngine::Get()
{
    static GraphicsEngine engine;
    return &engine;
}

SwapChain* GraphicsEngine::CreateSwapChain()
{
    return new SwapChain();
}

ID3D11Device* GraphicsEngine::GetDevice()
{
    return m_d3dDevice;
}

ID3D11DeviceContext* GraphicsEngine::GetDeviceContext()
{
    return m_immContext;
}

DeviceContext* GraphicsEngine::GetImmediateDeviceContext()
{
    return m_immDeviceContext;
}

VertexBuffer* GraphicsEngine::CreateVertexBuffer()
{
    return new VertexBuffer();
}

IndexBuffer* GraphicsEngine::CreateIndexBuffer()
{
    return new IndexBuffer();
}

ConstantBuffer* GraphicsEngine::CreateConstantBuffer()
{
    return new ConstantBuffer();
}

VertexShader* GraphicsEngine::CreateVertexShader(const void* shaderByteCode, size_t shaderByteSize)
{
    VertexShader* vs = new VertexShader();
    if(!vs->Init(shaderByteCode, shaderByteSize))
    {
        vs->Release();
        return nullptr;
    }
    
    return vs;
}

PixelShader* GraphicsEngine::CreatePixelShader(const void* shaderByteCode, size_t shaderByteSize)
{
    PixelShader* ps = new PixelShader();
    if(!ps->Init(shaderByteCode, shaderByteSize))
    {
        ps->Release();
        return nullptr;
    }
    
    return ps;
}
