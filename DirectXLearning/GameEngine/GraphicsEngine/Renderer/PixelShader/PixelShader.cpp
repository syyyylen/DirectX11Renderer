#include "PixelShader.h"
#include "..\..\GraphicsEngine.h"
#include "..\..\Renderer/Renderer.h"
#include "..\..\..\Logger/Log.h"
#include <exception>

PixelShader::PixelShader(Renderer* renderer, const void* shaderByteCode, size_t byteCodeSize) : m_renderer(renderer)
{
    if(!SUCCEEDED(m_renderer->m_d3dDevice->CreatePixelShader(shaderByteCode, byteCodeSize, nullptr, &m_ps)))
    {
        LOG("Pixel Shader Init failed");
        throw std::exception("Pixel Shader Init failed");
    }

    LOG("Pixel Shader Init Succeeded");
}

PixelShader::~PixelShader()
{
    if(m_ps)
        m_ps->Release();
}
