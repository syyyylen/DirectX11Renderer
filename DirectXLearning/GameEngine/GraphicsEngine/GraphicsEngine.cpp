#include "GraphicsEngine.h"
#include "../Logger/Log.h"

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
    UINT driver_type_index = ARRAYSIZE(driver_types);

    D3D_FEATURE_LEVEL feature_levels[]=
    {
        D3D_FEATURE_LEVEL_10_0
    };
    UINT num_feature_levels = ARRAYSIZE(feature_levels);

    HRESULT res = 0;
    
    for(UINT driver_type_index = 0 ; driver_type_index ; )
    {
        res = D3D11CreateDevice(NULL, driver_types[driver_type_index], NULL, NULL, feature_levels, num_feature_levels,
                            D3D11_SDK_VERSION, &m_d3dDevice, &m_featureLevel, &m_immContext);

        if(SUCCEEDED(res))
            break;

        driver_type_index++;
    }

    if(FAILED(res))
    {
        return false;
    }

    LOG("Graphics Engine Initialized");

    return true;
}

bool GraphicsEngine::Release()
{
    if(m_immContext)
        m_immContext->Release();
    
    if(m_d3dDevice)
        m_d3dDevice->Release();

    LOG("Graphics Engine Released");
    
    return true;
}

GraphicsEngine* GraphicsEngine::Get()
{
    static GraphicsEngine engine;
    return &engine;
}
