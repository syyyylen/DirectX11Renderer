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

    static Vector3 Lerp(const Vector3& start, const Vector3& end, float t)
    {
        Vector3 v;
        
        v.X = start.X*(1.0f - t) + end.X*t;
        v.Y = start.Y*(1.0f - t) + end.Y*t;
        v.Z = start.Z*(1.0f - t) + end.Z*t;

        return v;
    }

    Vector3 operator *(float num)
    {
        return Vector3(X*num, Y*num, Z*num);
    }

    Vector3 operator +(Vector3 vec)
    {
        return Vector3(X+vec.X, Y+vec.Y, Z+vec.Z);
    }

public:
    float X, Y, Z;
    
};