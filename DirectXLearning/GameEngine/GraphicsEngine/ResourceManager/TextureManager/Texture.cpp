#include "Texture.h"
#include <DirectXTex.h>
#include "..\..\GraphicsEngine.h"
#include "..\..\..\Logger/Log.h"

Texture::Texture(const wchar_t* fullPath) : Resource(fullPath)
{
    DirectX::ScratchImage imageData;
    HRESULT res = DirectX::LoadFromWICFile(fullPath, DirectX::WIC_FLAGS_NONE, nullptr, imageData);

    if(SUCCEEDED(res))
    {
        res = DirectX::CreateTexture(GraphicsEngine::Get()->GetRenderer()->m_d3dDevice, imageData.GetImages(), imageData.GetImageCount(), imageData.GetMetadata(), &m_texture);

        if(SUCCEEDED(res))
        {
            LOG("Texture Created, now trying to create shader resource view");
        }
        else
        {
            std::string errorMsg = std::system_category().message(res);
            LOG(errorMsg);
        }

        D3D11_SHADER_RESOURCE_VIEW_DESC desc = {};
        desc.Format = imageData.GetMetadata().format;
        desc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
        desc.Texture2D.MipLevels = imageData.GetMetadata().mipLevels;
        desc.Texture2D.MostDetailedMip = 0;
        res = GraphicsEngine::Get()->GetRenderer()->m_d3dDevice->CreateShaderResourceView(m_texture, &desc, &m_shaderResView);

        if(SUCCEEDED(res))
        {
            LOG("Shader Resource View creation succeded");
        }
        else
        {
            std::string errorMsg = std::system_category().message(res);
            LOG(errorMsg);
        }
    }
    else
    {
        std::string errorMsg = std::system_category().message(res);
        LOG(errorMsg);
        throw std::exception("Texture not created");
    }
}

Texture::~Texture()
{
    m_shaderResView->Release();
    m_texture->Release();
}
