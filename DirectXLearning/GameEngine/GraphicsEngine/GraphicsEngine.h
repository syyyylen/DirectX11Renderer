#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    // Init DX11 device
    bool Init();
    // Release loaded ressources
    bool Release();

    // Default simple shaders
    bool CreateShaders();
    bool SetShaders();
    
    bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
    void ReleaseCompiledShader();

public:
    static GraphicsEngine* Get();

public:
    SwapChain* CreateSwapChain();
    DeviceContext* GetImmediateDeviceContext();
    VertexBuffer* CreateVertexBuffer();
    VertexShader* CreateVertexShader(const void* shaderByteCode, size_t shaderByteSize);

private:
    ID3D11Device* m_d3dDevice;
    D3D_FEATURE_LEVEL m_featureLevel;
    
    IDXGIDevice* m_dxgiDevice;
    IDXGIAdapter* m_dxgiAdapter;
    IDXGIFactory* m_dxgiFactory;

    DeviceContext* m_immDeviceContext;

    ID3D11DeviceContext* m_immContext;

    ID3DBlob* m_blob = nullptr;
    
    ID3DBlob* m_vsblob = nullptr;
    ID3DBlob* m_psblob = nullptr;

    ID3D11VertexShader* m_vs = nullptr;
    ID3D11PixelShader* m_ps = nullptr;

private:
    friend class SwapChain;
    friend class VertexBuffer;
    friend class VertexShader;
};
