#pragma once
#include "../ResourceManager.h"

class TextureManager : ResourceManager
{
public:
    TextureManager();
    ~TextureManager() override;

    std::shared_ptr<Texture> CreateTextureFromFile(const wchar_t* filePath);
    
protected:
    virtual Resource* CreateResourceFromFileConcrete(const wchar_t* filePath) override;
};
