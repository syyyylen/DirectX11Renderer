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

    void ClearRenderTargetColor(const std::shared_ptr<SwapChain>& swapChain, float red, float green, float blue, float alpha);
    void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
    void DrawTriangleList(UINT vertexCount, UINT startVertexIndex);
    void DrawTriangleStrip(UINT vertexCount, UINT startVertexIndex);
    void DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation);
    
    void SetViewportSize(UINT width, UINT height);

    void SetVertexShader(const std::shared_ptr<VertexShader>& vertexShader);
    void SetPixelShader(const std::shared_ptr<PixelShader>& pixelShader);

    void SetTexture(const std::shared_ptr<VertexShader>& vertexShader, const std::shared_ptr<Texture>& texture);
    void SetTexture(const std::shared_ptr<PixelShader>& pixelShader, const std::shared_ptr<Texture>& texture);

    void SetConstantBuffer(const std::shared_ptr<VertexShader>& vertexShader, const std::shared_ptr<ConstantBuffer>& buffer);
    void SetConstantBuffer(const std::shared_ptr<PixelShader>& pixelShader, const std::shared_ptr<ConstantBuffer>& buffer);
    
private:
    ID3D11DeviceContext* m_deviceContext;
    Renderer* m_renderer = nullptr;

    friend class ConstantBuffer;
};
