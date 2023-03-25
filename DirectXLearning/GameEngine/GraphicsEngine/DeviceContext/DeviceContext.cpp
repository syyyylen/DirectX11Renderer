#include "DeviceContext.h"

#include "../SwapChain/SwapChain.h"
#include "../VertexBuffer/VertexBuffer.h"

DeviceContext::DeviceContext(ID3D11DeviceContext* deviceContext) : m_deviceContext(deviceContext)
{
    
}

DeviceContext::~DeviceContext()
{
}

void DeviceContext::ClearRenderTargetColor(SwapChain* swapChain, float red, float green, float blue, float alpha)
{
    FLOAT clearColor[] = { red, green, blue, alpha };
    m_deviceContext->ClearRenderTargetView(swapChain->m_rtv , clearColor);
    m_deviceContext->OMSetRenderTargets(1, &swapChain->m_rtv, NULL);
}

void DeviceContext::SetVertexBuffer(VertexBuffer* vertexBuffer)
{
    UINT stride = vertexBuffer->m_vertexSize;
    UINT offset = 0;
    m_deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer->m_buffer, &stride, &offset);
    m_deviceContext->IASetInputLayout(vertexBuffer->m_layout);
}

void DeviceContext::DrawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::SetViewportSize(UINT width, UINT height)
{
    D3D11_VIEWPORT vp = {};
    vp.Width = width;
    vp.Height = height;
    vp.MinDepth = 0.0f;
    vp.MaxDepth = 1.0f;
    m_deviceContext->RSSetViewports(1, &vp);
}

bool DeviceContext::Release()
{
    m_deviceContext->Release();
    delete this;

    return true;
}
