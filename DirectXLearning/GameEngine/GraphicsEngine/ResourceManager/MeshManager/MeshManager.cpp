#include "MeshManager.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

std::shared_ptr<Mesh> MeshManager::CreateMeshFromFile(const wchar_t* filePath)
{
    return std::static_pointer_cast<Mesh>(CreateResourceFromFile(filePath));
}

Resource* MeshManager::CreateResourceFromFileConcrete(const wchar_t* filePath)
{
    Mesh* mesh = nullptr;
    try
    {
        mesh = new Mesh(filePath);
    }
    catch (...){}

    return mesh;
}
