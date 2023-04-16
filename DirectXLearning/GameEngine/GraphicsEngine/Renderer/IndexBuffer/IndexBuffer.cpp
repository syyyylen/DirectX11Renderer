#include "IndexBuffer.h"

#include "../../../Logger/Log.h"
#include "..\Renderer.h"
#include <exception>

IndexBuffer::IndexBuffer(Renderer* renderer, void* listIndices, UINT listSize) : m_buffer(0), m_renderer(renderer)
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
    
    if(FAILED(m_renderer->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Buffer failed");
        throw std::exception("Create Buffer failed");
    }
}

IndexBuffer::~IndexBuffer()
{
    if(m_buffer)
        m_buffer->Release();
}

UINT IndexBuffer::GetIndexListSize()
{
    return m_listSize;
}