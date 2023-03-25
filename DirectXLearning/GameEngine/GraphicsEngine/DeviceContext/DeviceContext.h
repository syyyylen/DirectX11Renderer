﻿#pragma once
#include <d3d11.h>

class SwapChain;
class VertexBuffer;

class DeviceContext
{
public:
    DeviceContext(ID3D11DeviceContext* deviceContext);
    ~DeviceContext();

    void ClearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha);
    void SetVertexBuffer(VertexBuffer* vertexBuffer);
    void DrawTriangleList(UINT vertexCount, UINT startVertexIndex);
    void SetViewportSize(UINT width, UINT height);
    
    bool Release();

private:
    ID3D11DeviceContext* m_deviceContext;
};
