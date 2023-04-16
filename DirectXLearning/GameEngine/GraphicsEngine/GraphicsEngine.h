#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>

#include "Renderer/Renderer.h"

class SwapChain;
class DeviceContext;
class VertexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;
class IndexBuffer;

class GraphicsEngine
{
public:
    GraphicsEngine();
    ~GraphicsEngine();

    // Init DX11 device
    bool Init();
    // Release loaded ressources
    bool Release();

    Renderer* GetRenderer();

public:
    static GraphicsEngine* Get();

    Renderer* m_renderer = nullptr;
};
