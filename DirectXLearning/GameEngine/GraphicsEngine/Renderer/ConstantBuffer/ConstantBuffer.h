#pragma once
#include <d3d11.h>
#include "..\..\GraphicsEngine.h"
#include "..\..\Prerequisites.h"

class ConstantBuffer
{
public:
    ConstantBuffer(Renderer* renderer, void* buffer, UINT bufferSize);
    ~ConstantBuffer();

    void Update(DeviceContext* context, void* buffer);

private:
    ID3D11Buffer* m_buffer;
    Renderer* m_renderer = nullptr;

    friend class DeviceContext;
};
