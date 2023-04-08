#pragma once

class Vector3
{
public:
    Vector3() : X(0.0f), Y(0.0f), Z(0.0f)
    {
    }

    Vector3(float _x, float _y, float _z) : X(_x), Y(_y), Z(_z)
    {
    }

    Vector3(const Vector3& v) : X(v.X), Y(v.Y), Z(v.Z)
    {
    }

    ~Vector3()
    {
    }

    

public:
    float X, Y, Z;
    
};