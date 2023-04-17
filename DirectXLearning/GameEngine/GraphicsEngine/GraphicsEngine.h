#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Renderer/Renderer.h"
#include "ResourceManager/TextureManager/TextureManager.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;

class GraphicsEngine
{
private: 
    GraphicsEngine();
    ~GraphicsEngine();
    
public:
    Renderer* GetRenderer();
    TextureManager* GetTextureMgr();
    MeshManager* GetMeshMgr();
    void GetVertexMeshLayoutShaderByteCodeAndSize(void** ByteCode, size_t* Size);
    
    static GraphicsEngine* Get();
    static void Create();
    static void Release();

    Renderer* m_renderer = nullptr;
    TextureManager* m_textureManager = nullptr;
    MeshManager* m_meshManager = nullptr;
    static GraphicsEngine* m_graphicsEngine;

    unsigned char m_meshLayoutByteCode[1024];
    size_t m_meshLayoutSize;
};
