#pragma once
#include <d3d11.h>
#include "..\..\GraphicsEngine.h"
#include "..\..\Prerequisites.h"

class VertexBuffer
{
public:
    VertexBuffer(Renderer* renderer, void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize);
    ~VertexBuffer();

    UINT GetVertexListSize();

private:
    UINT m_vertexSize;
    UINT m_listSize;

    ID3D11Buffer* m_buffer;
    ID3D11InputLayout* m_layout;

    Renderer* m_renderer = nullptr;

    friend class DeviceContext;
};
