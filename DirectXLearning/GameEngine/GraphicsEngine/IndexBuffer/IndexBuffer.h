#pragma once
#include <d3d11.h>
#include "..\GraphicsEngine.h"

class DeviceContext;

class IndexBuffer
{
public:
    IndexBuffer();
    ~IndexBuffer();

    bool Load(void* listIndices, UINT listSize);
    UINT GetIndexListSize();
    bool Release();

private:
    UINT m_listSize;
    ID3D11Buffer* m_buffer;

    friend class DeviceContext;
};
