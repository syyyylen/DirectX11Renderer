#include "ConstantBuffer.h"
#include "../../../Logger/Log.h"
#include "../DeviceContext/DeviceContext.h"
#include <exception>

ConstantBuffer::ConstantBuffer(Renderer* renderer, void* buffer, UINT bufferSize) : m_renderer(renderer)
{
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = bufferSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = buffer;

    if(FAILED(m_renderer->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Constant Buffer failed");
        throw std::exception("Create Constant Buffer failed");
    }

    LOG("Create Constant Buffer suceeded");
}

ConstantBuffer::~ConstantBuffer()
{
    if(m_buffer)
        m_buffer->Release();
}

void ConstantBuffer::Update(std::shared_ptr<DeviceContext> context, void* buffer)
{
    context->m_deviceContext->UpdateSubresource(m_buffer, NULL, NULL, buffer, NULL, NULL);
}