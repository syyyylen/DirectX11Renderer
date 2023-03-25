#pragma once
#include <d3d11.h>
#include "..\GraphicsEngine.h"

class DeviceContext;

class VertexBuffer
{
public:
    VertexBuffer();
    ~VertexBuffer();

    bool Load(void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize);
    UINT GetVertexListSize();
    bool Release();

private:
    UINT m_vertexSize;
    UINT m_listSize;

    ID3D11Buffer* m_buffer;
    ID3D11InputLayout* m_layout;

    friend class DeviceContext;
};
