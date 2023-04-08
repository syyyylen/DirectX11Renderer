#pragma once
#include <memory>

#include "Vector3.h"

class Matrix4x4
{
public:
    Matrix4x4() 
    {
    }

    ~Matrix4x4()
    {
    }

    void SetIdentity()
    {
        memset(m_mat, 0, sizeof(float) * 16);
        m_mat[0][0] = 1.0f;
        m_mat[1][1] = 1.0f;
        m_mat[2][2] = 1.0f;
        m_mat[3][3] = 1.0f;
    }

    void SetTranslation(const Vector3& translation)
    {
        SetIdentity();
        m_mat[3][0] = translation.X;
        m_mat[3][1] = translation.Y;
        m_mat[3][2] = translation.Z;
    }

    void SetOrthoLH(float width, float height, float nearPlane, float farPlane)
    {
        SetIdentity();
        m_mat[0][0] = 2.0f / width;
        m_mat[1][1] = 2.0f / height;
        m_mat[2][2] = 1.0f / (farPlane - nearPlane);
        m_mat[3][2] = -(nearPlane / (farPlane - nearPlane));
    }

private:
    float m_mat[4][4] = {};

private:
    friend class Vector3;
    
};
