#ifndef __SEMIBASE_MATRIX_4X4_HEADER__
#define __SEMIBASE_MATRIX_4X4_HEADER__

#include "gfxMath.h"

namespace sbl {

struct Matrix44
{
    struct Elements
    {
        float _00, _01, _02, _03, _10, _11, _12, _13, _20, _21, _22, _23, _30, _31, _32, _33;
    };
    union // Declaration impossible
    {
        Elements m;
        struct // Declaration impossible
        {
            float  col[4];
            float& operator[](IN size_t index) { return col[index]; }
        } row[4];
    };
    float* operator[](IN size_t index);

    Matrix44(IN const float m00, IN const float m01, IN const float m02, IN const float m03, IN const float m10,
             IN const float m11, IN const float m12, IN const float m13, IN const float m20, IN const float m21,
             IN const float m22, IN const float m23, IN const float m30, IN const float m31, IN const float m32,
             IN const float m33);

    Matrix44();
    Matrix44(const Elements& param);
    Matrix44(const Matrix44& ref);
    Matrix44(const Matrix33& ref);

    Matrix44& operator=(const Matrix44& ref);
    Matrix44& operator=(const Matrix33& ref);
    // Matrix44& operator=(const Elements& param);

    Matrix44 operator+(IN const Matrix44& ref) const;
    Matrix44 operator-(IN const Matrix44& ref) const;
    Matrix44 operator*(IN const Matrix44& ref) const;
    Matrix44 operator*(IN const float scalar) const;
    Matrix44 operator/(IN const float scalar) const;
    Vector4  operator*(IN const Vector4& ref) const;

    Matrix44& operator+=(IN const Matrix44& ref);
    Matrix44& operator-=(IN const Matrix44& ref);
    Matrix44& operator*=(IN const Matrix44& ref);
    Matrix44& operator*=(IN const float scalar);
    Matrix44& operator/=(IN const float scalar);

    bool operator==(IN const Matrix44& ref) const;
    bool operator!=(IN const Matrix44& ref) const;

    static Matrix44 Zero();
    static Matrix44 Identity();
    static Matrix44 Translation(IN const float x, IN const float y, IN const float z);
    static Matrix44 Scale(IN const float x, IN const float y, IN const float z, IN const float w = 0);
    static Matrix44 RotationX(IN const float angle);
    static Matrix44 RotationY(IN const float angle);
    static Matrix44 RotationZ(IN const float angle);
    Matrix44        Transpose();
    Matrix44        Inverse();

    float Determinant();
    bool  IsSymmetric();
    bool  IsInvertible();
};

} // namespace sbl
#endif