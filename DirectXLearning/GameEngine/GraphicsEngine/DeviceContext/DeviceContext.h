#pragma once
#include <d3d11.h>

#include "../ConstantBuffer/ConstantBuffer.h"

class SwapChain;
class VertexBuffer;
class IndexBuffer;
class VertexShader;
class PixelShader;
class ConstantBuffer;

class DeviceContext
{
public:
    DeviceContext(ID3D11DeviceContext* deviceContext);
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
    
    bool Release();

private:
    ID3D11DeviceContext* m_deviceContext;

    friend class ConstantBuffer;
};
