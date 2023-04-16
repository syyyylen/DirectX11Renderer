#pragma once
#include <d3d11.h>
#include "..\..\Prerequisites.h"

class PixelShader
{
public:
    PixelShader(Renderer* renderer, const void* shaderByteCode, size_t byteCodeSize);
    ~PixelShader();
    
private:
    ID3D11PixelShader* m_ps;
    Renderer* m_renderer = nullptr;

    friend class Renderer;
    friend class DeviceContext;
};

