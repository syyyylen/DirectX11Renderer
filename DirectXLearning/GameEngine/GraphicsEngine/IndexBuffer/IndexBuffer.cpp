#include "IndexBuffer.h"

#include "../../Logger/Log.h"

IndexBuffer::IndexBuffer() : m_buffer(0)
{
}

IndexBuffer::~IndexBuffer()
{
}

bool IndexBuffer::Load(void* listIndices, UINT listSize)
{
    if(m_buffer)
        m_buffer->Release();

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = 4 * listSize;
    bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = listIndices;

    m_listSize = listSize;
    
    if(FAILED(GraphicsEngine::Get()->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Buffer failed");
        return false;
    }
    
    return true;
}

UINT IndexBuffer::GetIndexListSize()
{
    return m_listSize;
}

bool IndexBuffer::Release()
{
    if(m_buffer)
        m_buffer->Release();

    delete this;

    return true;
}
