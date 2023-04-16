#pragma once
#include <d3d11.h>
#include "..\..\GraphicsEngine.h"
#include "..\..\Prerequisites.h"

class IndexBuffer
{
public:
    IndexBuffer(Renderer* renderer, void* listIndices, UINT listSize);
    ~IndexBuffer();

    UINT GetIndexListSize();

private:
    UINT m_listSize;
    ID3D11Buffer* m_buffer;

    Renderer* m_renderer = nullptr;

    friend class DeviceContext;
};
