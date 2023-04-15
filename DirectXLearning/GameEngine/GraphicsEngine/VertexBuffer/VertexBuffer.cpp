#include "VertexBuffer.h"

#include "../../Logger/Log.h"

VertexBuffer::VertexBuffer() : m_layout(0), m_buffer(0)
{
}

VertexBuffer::~VertexBuffer()
{
}

bool VertexBuffer::Load(void* listVertices, UINT vertexSize, UINT listSize, void* shaderByteCode, UINT shaderByteSize)
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
    
    if(FAILED(GraphicsEngine::Get()->m_d3dDevice->CreateBuffer(&bufferDesc, &initData, &m_buffer)))
    {
        LOG("Create Buffer failed");
        return false;
    }

    D3D11_INPUT_ELEMENT_DESC layout[]=
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 1, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };

    UINT layoutSize = ARRAYSIZE(layout);
    
    if(FAILED(GraphicsEngine::Get()->m_d3dDevice->CreateInputLayout(layout, layoutSize, shaderByteCode, shaderByteSize, &m_layout)))
    {
        LOG("Create Input Layout failed");
        return false;
    }
    
    return true;
}

UINT VertexBuffer::GetVertexListSize()
{
    return m_listSize;
}

bool VertexBuffer::Release()
{
    if(m_layout)
        m_layout->Release();

    if(m_buffer)
        m_buffer->Release();

    delete this;

    return true;
}
