#pragma once
#include <vector>
#include <cmath>

namespace Transformation
{
    using Mat = std::vector<std::vector<float>>; // 4x4

    inline Mat identity4()
    {
        return {
            {1.f,0.f,0.f,0.f},
            {0.f,1.f,0.f,0.f},
            {0.f,0.f,1.f,0.f},
            {0.f,0.f,0.f,1.f}
        };
    }

    inline Mat translate3D(float dx, float dy, float dz)
    {
        return {
            {1.f,0.f,0.f,dx },
            {0.f,1.f,0.f,dy },
            {0.f,0.f,1.f,dz },
            {0.f,0.f,0.f,1.f}
        };
    }

    inline Mat scale3D(float sx, float sy, float sz)
    {
        return {
            {sx ,0.f,0.f,0.f},
            {0.f,sy ,0.f,0.f},
            {0.f,0.f,sz ,0.f},
            {0.f,0.f,0.f,1.f}
        };
    }

    // 2D rotation about Z-axis (triangle lies in XY plane)
    inline Mat rotateZ(float degrees)
    {
        const float r = degrees * 3.14159265358979323846f / 180.f;
        const float c = std::cos(r);
        const float s = std::sin(r);
        return {
            { c,-s,0.f,0.f},
            { s, c,0.f,0.f},
            {0.f,0.f,1.f,0.f},
            {0.f,0.f,0.f,1.f}
        };
    }

    inline Mat multiply(const Mat& A, const Mat& B)
    {
        Mat C(4, std::vector<float>(4, 0.f));
        for (int i = 0;i < 4;++i)
            for (int j = 0;j < 4;++j)
                for (int k = 0;k < 4;++k)
                    C[i][j] += A[i][k] * B[k][j];
        return C;
    }

    inline void toArrayRowMajor(const Mat& M, float out[16])
    {
        out[0] = M[0][0]; out[1] = M[0][1]; out[2] = M[0][2]; out[3] = M[0][3];
        out[4] = M[1][0]; out[5] = M[1][1]; out[6] = M[1][2]; out[7] = M[1][3];
        out[8] = M[2][0]; out[9] = M[2][1]; out[10] = M[2][2]; out[11] = M[2][3];
        out[12] = M[3][0]; out[13] = M[3][1]; out[14] = M[3][2]; out[15] = M[3][3];
    }
}
