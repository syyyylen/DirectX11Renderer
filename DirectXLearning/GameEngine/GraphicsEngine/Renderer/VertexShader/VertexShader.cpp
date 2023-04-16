#include "VertexShader.h"
#include "..\..\GraphicsEngine.h"
#include "..\..\Renderer/Renderer.h"
#include "..\..\..\Logger/Log.h"
#include <exception>

VertexShader::VertexShader(Renderer* renderer, const void* shaderByteCode, size_t byteCodeSize) : m_renderer(renderer)
{
    if(!SUCCEEDED(m_renderer->m_d3dDevice->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, &m_vs)))
    {
        LOG("Vertex Shader Init failed");
        throw std::exception("Vertex Shader Init failed");
    }

    LOG("Vertex Shader Init Succeeded");
}

VertexShader::~VertexShader()
{
    if(m_vs)
        m_vs->Release();
}
