#pragma once

#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
    VertexMesh() :Position(), Texcoord(), Normal()
    {
    }
    
    VertexMesh(Vector3 position, Vector2 texcoord, Vector3 normal) :Position(position), Texcoord(texcoord), Normal(normal)
    {
    }
    
    VertexMesh(const VertexMesh& vertex) :Position(vertex.Position), Texcoord(vertex.Texcoord), Normal(vertex.Normal)
    {
    }

    ~VertexMesh()
    {
    }

public:
    Vector3 Position;
    Vector2 Texcoord;
    Vector3 Normal;
};
