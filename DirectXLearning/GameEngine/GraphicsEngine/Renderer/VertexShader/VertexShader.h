#pragma once
#include <d3d11.h>
#include "..\..\Prerequisites.h"

class VertexShader
{
public:
    VertexShader(Renderer* renderer, const void* shaderByteCode, size_t byteCodeSize);
    ~VertexShader();
    
private:
    ID3D11VertexShader* m_vs;
    Renderer* m_renderer = nullptr;

    friend class Renderer;
    friend class DeviceContext;
};
