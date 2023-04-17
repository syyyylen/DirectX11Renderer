#pragma once
#include "../ResourceManager.h"
#include "Mesh.h"

class MeshManager : public ResourceManager
{
public:
    MeshManager();
    ~MeshManager();

    std::shared_ptr<Mesh> CreateMeshFromFile(const wchar_t* filePath);
    
protected:
    virtual Resource* CreateResourceFromFileConcrete(const wchar_t* filePath) override;
};
