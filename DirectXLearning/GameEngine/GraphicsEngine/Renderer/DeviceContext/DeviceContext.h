#pragma once
#include <d3d11.h>

#include "../Renderer.h"
#include "../ConstantBuffer/ConstantBuffer.h"
#include "..\..\Prerequisites.h"

class DeviceContext
{
public:
    DeviceContext(ID3D11DeviceContext* deviceContext, Renderer* renderer);
    ~DeviceContext();

    void ClearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha);
    void SetVertexBuffer(VertexBuffer* vertexBuffer);
    void SetIndexBuffer(IndexBuffer* indexBuffer);
    void DrawTriangleList(UINT vertexCount, UINT startVertexIndex);
    void DrawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
    void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
    
    void SetViewportSize(UINT width, UINT height);

    void SetVertexShader(VertexShader* vertexShader);
    void SetPixelShader(PixelShader* pixelShader);

    void SetConstantBuffer(VertexShader* vertexShader, ConstantBuffer* buffer);
    void SetConstantBuffer(PixelShader* vertexShader, ConstantBuffer* buffer);
    
private:
    ID3D11DeviceContext* m_deviceContext;
    Renderer* m_renderer = nullptr;

    friend class ConstantBuffer;
};
