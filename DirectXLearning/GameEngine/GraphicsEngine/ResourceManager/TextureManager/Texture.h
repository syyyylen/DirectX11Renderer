#pragma once
#include "../Resource.h"
#include <d3d11.h>

class Texture : public Resource
{
public:
    Texture(const wchar_t* fullPath);
    ~Texture();

private:
    ID3D11Resource* m_texture = nullptr;
    ID3D11ShaderResourceView* m_shaderResView = nullptr;

    friend class DeviceContext;
};
