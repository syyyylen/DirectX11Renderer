#pragma once
#include <d3d11.h>

class SwapChain;
class DeviceContext;

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    // Init DX11 device
    bool Init();
    // Release loaded ressources
    bool Release();

public:
    static GraphicsEngine* Get();

public:
    SwapChain* CreateSwapChain();
    DeviceContext* GetImmediateDeviceContext();

private:
    ID3D11Device* m_d3dDevice;
    D3D_FEATURE_LEVEL m_featureLevel;
    
    IDXGIDevice* m_dxgiDevice;
    IDXGIAdapter* m_dxgiAdapter;
    IDXGIFactory* m_dxgiFactory;

    DeviceContext* m_immDeviceContext;

private:
    friend class SwapChain;
};
