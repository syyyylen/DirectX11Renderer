#include "PixelShader.h"
#include "..\GraphicsEngine.h"
#include "..\..\Logger/Log.h"

PixelShader::PixelShader()
{
}

PixelShader::~PixelShader()
{
}

void PixelShader::Release()
{
    if(m_ps)
        m_ps->Release();
    
    delete this;
}

bool PixelShader::Init(const void* shaderByteCode, size_t byteCodeSize)
{
    if(!SUCCEEDED(GraphicsEngine::Get()->m_d3dDevice->CreatePixelShader(shaderByteCode, byteCodeSize, nullptr, &m_ps)))
    {
        LOG("Pixel Shader Init failed");
        return false;
    }

    LOG("Pixel Shader Init Succeeded");
    
    return true;
}
