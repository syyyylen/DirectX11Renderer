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

    void SetScale(const Vector3& scale)
    {
        SetIdentity();
        m_mat[0][0] = scale.X;
        m_mat[1][1] = scale.Y;
        m_mat[2][2] = scale.Z;
    }

    void SetRotationX(float x)
    {
        m_mat[1][1] = cos(x);
        m_mat[1][2] = sin(x);
        m_mat[2][1] = -sin(x);
        m_mat[2][2] = cos(x);
    }

    void SetRotationY(float y)
    {
        m_mat[0][0] = cos(y);
        m_mat[0][2] = -sin(y);
        m_mat[2][0] = sin(y);
        m_mat[2][2] = cos(y);
    }

    void SetRotationZ(float z)
    {
        m_mat[0][0] = cos(z);
        m_mat[0][1] = sin(z);
        m_mat[1][0] = -sin(z);
        m_mat[1][1] = cos(z);
    }

    void SetOrthoLH(float width, float height, float nearPlane, float farPlane)
    {
        SetIdentity();
        m_mat[0][0] = 2.0f / width;
        m_mat[1][1] = 2.0f / height;
        m_mat[2][2] = 1.0f / (farPlane - nearPlane);
        m_mat[3][2] = -(nearPlane / (farPlane - nearPlane));
    }

    void operator *=(const Matrix4x4& matrix)
    {
        Matrix4x4 out;
        for(int i = 0; i < 4; i++)
        {
            for(int j = 0; j < 4; j++)
            {
                out.m_mat[i][j] =
                    m_mat[i][0] * matrix.m_mat[0][j] + m_mat[i][1] * matrix.m_mat[1][j] +
                    m_mat[i][2] * matrix.m_mat[2][j] + m_mat[i][3] * matrix.m_mat[3][j];
            }
        }
        memcpy(m_mat, out.m_mat, sizeof(float) * 16);
    }

private:
    float m_mat[4][4] = {};

private:
    friend class Vector3;
    
};
