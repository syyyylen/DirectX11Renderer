#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    // Init DX11 device
    bool Init();
    // Release loaded ressources
    bool Release();

    bool CreateShaders();
    bool SetShaders();
    void GetShaderBufferAndSize(void** byteCode, UINT* size);

public:
    static GraphicsEngine* Get();

public:
    SwapChain* CreateSwapChain();
    DeviceContext* GetImmediateDeviceContext();
    VertexBuffer* CreateVertexBuffer();

private:
    ID3D11Device* m_d3dDevice;
    D3D_FEATURE_LEVEL m_featureLevel;
    
    IDXGIDevice* m_dxgiDevice;
    IDXGIAdapter* m_dxgiAdapter;
    IDXGIFactory* m_dxgiFactory;

    DeviceContext* m_immDeviceContext;

    ID3D11DeviceContext* m_immContext;

    ID3DBlob* m_vsblob;
    ID3DBlob* m_psblob;

    ID3D11VertexShader* m_vs;
    ID3D11PixelShader* m_ps;

private:
    friend class SwapChain;
    friend class VertexBuffer;
};
