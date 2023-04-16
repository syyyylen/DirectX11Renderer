#pragma once

class Vector2
{
public:
    Vector2() :X(0), Y(0)
    {
    }
    Vector2(float x, float y) :X(x), Y(y)
    {
    }
    Vector2(const Vector2& vector) :X(vector.X), Y(vector.Y)
    {
    }

    Vector2 operator *(float num)
    {
        return Vector2(X*num, Y*num);
    }

    Vector2 operator +(Vector2 vec)
    {
        return Vector2(X + vec.X, Y + vec.Y);
    }

    ~Vector2()
    {
    }

public:
    float X, Y;
};
