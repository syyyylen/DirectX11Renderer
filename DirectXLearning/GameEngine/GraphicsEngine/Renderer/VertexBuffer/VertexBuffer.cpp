#include "VertexBuffer.h"

#include "../Renderer.h"
#include "../../../Logger/Log.h"
#include <exception>

VertexBuffer::VertexBuffer(Renderer* renderer, void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize) : m_layout(0), m_buffer(0), m_renderer(renderer)
{
    if(m_buffer)
        m_buffer->Release();

    if(m_layout)
        m_layout->Release();
    
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = vertexSize * listSize;
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    bufferDesc.CPUAccessFlags = 0;
    bufferDesc.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = listVertices;

    m_vertexSize = vertexSize;
    m_listSize = listSize;
    
    if(FAILED(m_renderer->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Buffer failed");
        throw std::exception("Create Buffer failed");
    }

    D3D11_INPUT_ELEMENT_DESC layout[]=
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT layoutSize = ARRAYSIZE(layout);
    
    if(FAILED(m_renderer->m_d3dDevice->CreateInputLayout(layout, layoutSize, shaderByteCode, shaderByteSize, &m_layout)))
    {
        LOG("Create Input Layout failed");
        throw std::exception("Create Input Layout failed");
    }
}

VertexBuffer::~VertexBuffer()
{
    if(m_layout)
        m_layout->Release();

    if(m_buffer)
        m_buffer->Release();
}

UINT VertexBuffer::GetVertexListSize()
{
    return m_listSize;
}