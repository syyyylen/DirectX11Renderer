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

    void ClearRenderTargetColor(std::shared_ptr<SwapChain> swapChain, float red, float green, float blue, float alpha);
    void SetVertexBuffer(std::shared_ptr<VertexBuffer> vertexBuffer);
    void SetIndexBuffer(std::shared_ptr<IndexBuffer> indexBuffer);
    void DrawTriangleList(UINT vertexCount, UINT startVertexIndex);
    void DrawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
    void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
    
    void SetViewportSize(UINT width, UINT height);

    void SetVertexShader(std::shared_ptr<VertexShader> vertexShader);
    void SetPixelShader(std::shared_ptr<PixelShader> pixelShader);

    void SetConstantBuffer(std::shared_ptr<VertexShader> vertexShader, std::shared_ptr<ConstantBuffer> buffer);
    void SetConstantBuffer(std::shared_ptr<PixelShader> pixelShader, std::shared_ptr<ConstantBuffer> buffer);
    
private:
    ID3D11DeviceContext* m_deviceContext;
    Renderer* m_renderer = nullptr;

    friend class ConstantBuffer;
};
