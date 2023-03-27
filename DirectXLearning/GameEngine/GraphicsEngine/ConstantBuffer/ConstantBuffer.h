#pragma once
#include <d3d11.h>
#include "..\GraphicsEngine.h"

class DeviceContext;

class ConstantBuffer
{
public:
    ConstantBuffer();
    ~ConstantBuffer();

    bool Load(void* buffer, UINT bufferSize);
    void Update(DeviceContext* context, void* buffer);
    bool Release();

private:
    ID3D11Buffer* m_buffer;

    friend class DeviceContext;
};
