#include "GraphicsEngine.h"
#include"../Logger/Log.h"
#include "SwapChain/SwapChain.h"
#include "DeviceContext/DeviceContext.h"
#include "VertexBuffer/VertexBuffer.h"

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

bool GraphicsEngine::CreateShaders()
{
    ID3DBlob* errblob = nullptr;
    D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "vsmain", "vs_5_0", NULL, NULL, &m_vsblob, &errblob);
    D3DCompileFromFile(L"shader.fx", nullptr, nullptr, "psmain", "ps_5_0", NULL, NULL, &m_psblob, &errblob);
    m_d3dDevice->CreateVertexShader(m_vsblob->GetBufferPointer(), m_vsblob->GetBufferSize(), nullptr, &m_vs);
    m_d3dDevice->CreatePixelShader(m_psblob->GetBufferPointer(), m_psblob->GetBufferSize(), nullptr, &m_ps);
    
    return true;
}

bool GraphicsEngine::SetShaders()
{
    m_immContext->VSSetShader(m_vs, nullptr, 0);
    m_immContext->PSSetShader(m_ps, nullptr, 0);
    
    return true;
}

void GraphicsEngine::GetShaderBufferAndSize(void** byteCode, UINT* size)
{
    *byteCode = this->m_vsblob->GetBufferPointer();
    *size = (UINT)this->m_vsblob->GetBufferSize();
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

DeviceContext* GraphicsEngine::GetImmediateDeviceContext()
{
    return m_immDeviceContext;
}

VertexBuffer* GraphicsEngine::CreateVertexBuffer()
{
    return new VertexBuffer();
}
