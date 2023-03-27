#include "ConstantBuffer.h"
#include "../../Logger/Log.h"
#include "../DeviceContext/DeviceContext.h"

ConstantBuffer::ConstantBuffer()
{
}

ConstantBuffer::~ConstantBuffer()
{
}

bool ConstantBuffer::Load(void* buffer, UINT bufferSize)
{
    if(m_buffer)
        m_buffer->Release();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = bufferSize;
    bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = buffer;

    if(FAILED(GraphicsEngine::Get()->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Constant Buffer failed");
        return false;
    }

    LOG("Create Constant Buffer suceeded");

    return true;
}

void ConstantBuffer::Update(DeviceContext* context, void* buffer)
{
    context->m_deviceContext->UpdateSubresource(m_buffer, NULL, NULL, buffer, NULL, NULL);
}

bool ConstantBuffer::Release()
{
    if(m_buffer)
        m_buffer->Release();

    delete this;
    
    return true;
}
