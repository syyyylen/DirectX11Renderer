#include "Mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <locale>
#include <codecvt>

#include "..\..\..\GraphicsEngine/GraphicsEngine.h"
#include "../../../Logger/Log.h"
#include "../../../Math/VertexMesh.h"

Mesh::Mesh(const wchar_t* fullPath): Resource(fullPath)
{
    tinyobj::attrib_t attributes;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string error;
    std::string inputfile = std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(fullPath);

    bool res = tinyobj::LoadObj(&attributes, &shapes, &materials, &warn, &error, inputfile.c_str());

    if(!error.empty() || !res)
    {
        LOG("Mesh failed to load : ");
        LOG(error);
        throw std::exception("Mesh not loaded successfully");
    }

    if(shapes.size() > 1)
    {
        LOG("Mesh has multiple shapes!");
        throw std::exception("Mesh has multiple shapes!");
    }

    std::vector<VertexMesh> verticesList;
    std::vector<unsigned int> indicesList;

    for(size_t s = 0; s < shapes.size(); s++)
    {
        size_t indexOffset = 0;
        verticesList.reserve(shapes[s].mesh.indices.size());
        indicesList.reserve(shapes[s].mesh.indices.size());
        
        for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
        {
            unsigned char numFaceVerts = shapes[s].mesh.num_face_vertices[f];

            for(unsigned char v = 0; v < numFaceVerts; v++)
            {
                tinyobj::index_t index = shapes[s].mesh.indices[indexOffset + v];
                
                tinyobj::real_t vx = attributes.vertices[index.vertex_index * 3 + 0];
                tinyobj::real_t vy = attributes.vertices[index.vertex_index * 3 + 1];
                tinyobj::real_t vz = attributes.vertices[index.vertex_index * 3 + 2];
                
                tinyobj::real_t tx = attributes.texcoords[index.texcoord_index * 2 + 0];
                tinyobj::real_t ty = attributes.texcoords[index.texcoord_index * 2 + 1];

                VertexMesh vertex(Vector3(vx, vy, vz), Vector2(tx, ty));
                verticesList.push_back(vertex);

                indicesList.push_back((unsigned int)indexOffset + v);
            }

            indexOffset += numFaceVerts;
        }
    }

    void* shaderByteCode = nullptr;
    size_t shaderSize = 0;
    GraphicsEngine::Get()->GetVertexMeshLayoutShaderByteCodeAndSize(&shaderByteCode, &shaderSize);
    m_vertexBuffer = GraphicsEngine::Get()->GetRenderer()->CreateVertexBuffer(&verticesList[0], sizeof(VertexMesh),
        (UINT)verticesList.size(), shaderByteCode, (UINT)shaderSize);
    
    m_indexBuffer = GraphicsEngine::Get()->GetRenderer()->CreateIndexBuffer(&indicesList[0], (UINT)indicesList.size());
}

Mesh::~Mesh()
{
}

const std::shared_ptr<VertexBuffer>& Mesh::GetVertexBuffer()
{
    return m_vertexBuffer;
}

const std::shared_ptr<IndexBuffer>& Mesh::GetIndexBuffer()
{
    return m_indexBuffer;
}
