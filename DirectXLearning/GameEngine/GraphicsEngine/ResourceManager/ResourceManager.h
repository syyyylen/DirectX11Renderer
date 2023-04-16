#pragma once
#include <unordered_map>
#include <string>
#include "..\Prerequisites.h"

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();

    std::shared_ptr<Resource> CreateResourceFromFile(const wchar_t* filePath);

protected:
    virtual Resource* CreateResourceFromFileConcrete(const wchar_t* filePath) = 0;

private:
    std::unordered_map<std::wstring, std::shared_ptr<Resource>> m_resourcesMap;
};
