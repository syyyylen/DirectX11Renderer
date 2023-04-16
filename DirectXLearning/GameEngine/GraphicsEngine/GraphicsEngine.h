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
    static GraphicsEngine* Get();
    static void Create();
    static void Release();

    Renderer* m_renderer = nullptr;
    TextureManager* m_textureManager = nullptr;
    static GraphicsEngine* m_graphicsEngine;
};
