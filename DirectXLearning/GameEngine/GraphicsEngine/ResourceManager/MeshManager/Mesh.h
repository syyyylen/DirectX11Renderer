#pragma once
#include "..\Resource.h"
#include "..\..\Renderer/VertexBuffer/VertexBuffer.h"
#include "..\..\Renderer/IndexBuffer/IndexBuffer.h"

class Mesh : public Resource
{
public:
    Mesh(const wchar_t* fullPath);
    ~Mesh();

    const std::shared_ptr<VertexBuffer>& GetVertexBuffer();
    const std::shared_ptr<IndexBuffer>& GetIndexBuffer();

private:
    std::shared_ptr<VertexBuffer> m_vertexBuffer;
    std::shared_ptr<IndexBuffer> m_indexBuffer;

    friend class DeviceContext;
};
