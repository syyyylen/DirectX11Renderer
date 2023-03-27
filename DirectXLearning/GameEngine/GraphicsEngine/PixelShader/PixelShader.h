#pragma once
#include <d3d11.h>

class GraphicsEngine;
class DeviceContext;

class PixelShader
{
public:
    PixelShader();
    ~PixelShader();
    
    void Release();

    private :
        bool Init(const void* shaderByteCode, size_t byteCodeSize);

private:
    ID3D11PixelShader* m_ps;

    friend class GraphicsEngine;
    friend class DeviceContext;
};

