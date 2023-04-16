#include "ResourceManager.h"
#include "..\..\Logger/Log.h"


#if __cplusplus <= 201402L 
#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <experimental/filesystem>
#endif

#if __cplusplus >= 201703L
#include <filesystem>
#endif


ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

std::shared_ptr<Resource> ResourceManager::CreateResourceFromFile(const wchar_t* filePath)
{
#if (_MSC_VER >= 1900 && _MSC_VER <= 1916)  || ( _MSC_VER >= 1920 && __cplusplus <= 201402L) 
    std::wstring full_path = std::experimental::filesystem::absolute(filePath);
#endif

#if _MSC_VER >= 1920 && __cplusplus > 201402L 
    std::wstring full_path = std::filesystem::absolute(file_path);
#endif

    auto it = m_resourcesMap.find(full_path);

    if (it != m_resourcesMap.end())
        return it->second;

    Resource* raw_res = this->CreateResourceFromFileConcrete(full_path.c_str());

    if (raw_res)
    {
        std::shared_ptr<Resource> res(raw_res);
        m_resourcesMap[full_path] = res;
        return res;
    }

    return nullptr;
}
