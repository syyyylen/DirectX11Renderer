#include "DeviceContext.h"

#include "../SwapChain/SwapChain.h"
#include "../VertexBuffer/VertexBuffer.h"
#include "../IndexBuffer/IndexBuffer.h"
#include "../VertexShader/VertexShader.h"
#include "../PixelShader/PixelShader.h"
#include "../ConstantBuffer/ConstantBuffer.h"

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

void DeviceContext::SetIndexBuffer(IndexBuffer* indexBuffer)
{
    m_deviceContext->IASetIndexBuffer(indexBuffer->m_buffer, DXGI_FORMAT_R32_UINT, 0);
}

void DeviceContext::DrawTriangleList(UINT vertexCount, UINT startVertexIndex)
{
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::DrawTriangleStrip(UINT vertexCount, UINT startVertexIndex)
{
    m_deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
    m_deviceContext->Draw(vertexCount, startVertexIndex);
}

void DeviceContext::DrawIndexedTriangleList(UINT indexCount, UINT startVertexIndex, UINT startIndexLocation)
{
    m_deviceContext->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
    m_deviceContext->DrawIndexed(indexCount, startIndexLocation, startVertexIndex);
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

void DeviceContext::SetVertexShader(VertexShader* vertexShader)
{
    m_deviceContext->VSSetShader(vertexShader->m_vs, nullptr, 0);
}

void DeviceContext::SetPixelShader(PixelShader* pixelShader)
{
    m_deviceContext->PSSetShader(pixelShader->m_ps, nullptr, 0);
}

void DeviceContext::SetConstantBuffer(VertexShader* vertexShader, ConstantBuffer* buffer)
{
    m_deviceContext->VSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

void DeviceContext::SetConstantBuffer(PixelShader* vertexShader, ConstantBuffer* buffer)
{
    m_deviceContext->PSSetConstantBuffers(0, 1, &buffer->m_buffer);
}

bool DeviceContext::Release()
{
    m_deviceContext->Release();
    delete this;

    return true;
}
