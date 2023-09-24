#ifndef SBL_MATRIX_3X3_HEADER__
#define SBL_MATRIX_3X3_HEADER__

#include "gfxMath.h"

namespace sbl {

struct Matrix33
{
    struct Elements
    {
        float _00, _01, _02, _10, _11, _12, _20, _21, _22;
    };
    union // Declaration impossible
    {
        Elements m;
        struct // Declaration impossible
        {
            float  col[3];
            float& operator[](IN size_t index) { return col[index]; }
        } row[3];
    };
    float* operator[](IN size_t index);

    Matrix33(IN const float m00, IN const float m01, IN const float m02, IN const float m10, IN const float m11,
             IN const float m12, IN const float m20, IN const float m21, IN const float m22);

    Matrix33();
    Matrix33(IN const Matrix33& ref);
    Matrix33(IN const Elements& param);

    Matrix33& operator=(IN const Matrix33& ref);
    // Matrix33& operator=(const Elements& param);

    Matrix33 operator+(IN const Matrix33& ref) const;
    Matrix33 operator-(IN const Matrix33& ref) const;
    Matrix33 operator*(IN const Matrix33& ref) const;
    Matrix33 operator*(IN const float scalar) const;
    Matrix33 operator/(IN const float scalar) const;
    Vector3  operator*(IN const Vector3& ref) const;

    Matrix33& operator+=(IN const Matrix33& ref);
    Matrix33& operator-=(IN const Matrix33& ref);
    Matrix33& operator*=(IN const Matrix33& ref);
    Matrix33& operator*=(IN const float scalar);
    Matrix33& operator/=(IN const float scalar);

    bool operator==(IN const Matrix33& ref) const;
    bool operator!=(IN const Matrix33& ref) const;

    static Matrix33 Zero();
    static Matrix33 Identity();
    static Matrix33 Translation(IN const float x, IN const float y);
    static Matrix33 Scale(IN const float x, IN const float y, IN const float z = 0);
    static Matrix33 RotationX(IN const float angle);
    static Matrix33 RotationY(IN const float angle);
    static Matrix33 RotationZ(IN const float angle);
    Matrix33        Transpose();
    Matrix33        Inverse();

    float Determinant();
    bool  IsSymmetric();
    bool  IsInvertible();
};

} // namespace sbl
#endif
