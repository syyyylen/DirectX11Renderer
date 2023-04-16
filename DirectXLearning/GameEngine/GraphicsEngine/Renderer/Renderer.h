#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include "..\Prerequisites.h"

class Renderer
{
public:
    Renderer();
    ~Renderer();

    // Init DX11 device
    bool Init();
    // Release loaded ressources
    bool Release();

    bool CompileVertexShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
    bool CompilePixelShader(const wchar_t* fileName, const char* entryPointName, void** shaderByteCode, size_t* byteCodeSize);
    
    void ReleaseCompiledShader();

public:
    SwapChain* CreateSwapChain(HWND hwnd, UINT width, UINT height);
    ID3D11Device* GetDevice();
    ID3D11DeviceContext* GetDeviceContext();
    DeviceContext* GetImmediateDeviceContext();
    VertexBuffer* CreateVertexBuffer(void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize);
    IndexBuffer* CreateIndexBuffer(void* listIndices, UINT listSize);
    ConstantBuffer* CreateConstantBuffer(void* buffer, UINT bufferSize);
    VertexShader* CreateVertexShader(const void* shaderByteCode, size_t shaderByteSize);
    PixelShader* CreatePixelShader(const void* shaderByteCode, size_t shaderByteSize);

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
    friend class PixelShader;
    friend class ConstantBuffer;
    friend class IndexBuffer;
};
