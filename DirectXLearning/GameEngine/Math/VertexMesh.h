#pragma once

#include "Vector2.h"
#include "Vector3.h"

class VertexMesh
{
public:
    VertexMesh() :Position(), Texcoord()
    {
    }
    VertexMesh(Vector3 position, Vector2 texcoord) :Position(position), Texcoord(texcoord)
    {
    }
    VertexMesh(const VertexMesh& vertex) :Position(vertex.Position), Texcoord(vertex.Texcoord)
    {
    }

    ~VertexMesh()
    {
    }

public:
    Vector3 Position;
    Vector2 Texcoord;
};
