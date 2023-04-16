#include "TextureManager.h"
#include "Texture.h"
#include "..\..\..\Logger/Log.h"

TextureManager::TextureManager(): ResourceManager()
{
}

TextureManager::~TextureManager()
{
}

std::shared_ptr<Texture> TextureManager::CreateTextureFromFile(const wchar_t* filePath)
{
    return std::static_pointer_cast<Texture>(CreateResourceFromFile(filePath));
}

Resource* TextureManager::CreateResourceFromFileConcrete(const wchar_t* filePath)
{
    Texture* tex = nullptr;
    try
    {
        tex = new Texture(filePath);
    }
    catch (...){}

    return tex;
}
