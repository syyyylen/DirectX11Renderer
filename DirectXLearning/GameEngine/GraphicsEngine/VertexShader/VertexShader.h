#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class VertexShader
{
public:
    VertexShader();
    ~VertexShader();
    
    void Release();

private :
    bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
    ID3D11VertexShader* m_vs;

    friend class GraphicsEngine;
    friend class DeviceContext;
};
