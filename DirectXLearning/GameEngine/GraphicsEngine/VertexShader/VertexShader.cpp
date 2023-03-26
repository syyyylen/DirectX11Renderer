#include "VertexShader.h"
#include "..\GraphicsEngine.h"
#include "..\..\Logger/Log.h"

VertexShader::VertexShader()
{
}

VertexShader::~VertexShader()
{
}

void VertexShader::Release()
{
    if(m_vs)
        m_vs->Release();
    
    delete this;
}

bool VertexShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
    if(!SUCCEEDED(GraphicsEngine::Get()->m_d3dDevice->CreateVertexShader(shaderByteCode, byteCodeSize, nullptr, &m_vs)))
    {
        LOG("Pixel Shader Init failed");
        return false;
    }

    LOG("Pixel Shader Init Succeeded");
    
    return true;
}
