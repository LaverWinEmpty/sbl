#ifndef SBL_VECTOR_3D_HEADER__
#define SBL_VECTOR_3D_HEADER__

#include "vector2.h"

namespace sbl {

struct Vector3: Vector2
{
    float z;

    Vector3();
    Vector3(IN const float x, IN const float y, IN const float z);
    Vector3(IN const Vector3& ref);
    Vector3(IN const Vector2& ref);

    Vector3& operator=(const Vector3& ref);

    Vector3 operator+(IN const Vector3& ref) const;
    Vector3 operator-(IN const Vector3& ref) const;
    Vector3 operator*(IN const float scalar) const;
    Vector3 operator/(IN const float scalar) const;
    float   operator*(IN const Vector3& ref) const;

    Vector3& operator+=(IN const Vector3& ref);
    Vector3& operator-=(IN const Vector3& ref);
    Vector3& operator*=(IN const float scalar);
    Vector3& operator/=(IN const float scalar);

    Vector3  operator^(IN const Vector3& ref) const;
    Vector3& operator^=(IN const Vector3& ref);

    Vector3  operator*(IN const Matrix33& ref) const;
    Vector4  operator*(IN const Matrix44& ref) const;
    Vector3  operator*(IN const Quaternion& ref) const;
    Vector3& operator*=(IN const Matrix33& ref);
    Vector3& operator*=(IN const Matrix44& ref);
    Vector3& operator*=(IN const Quaternion& ref);

    Vector3 operator-() const;

    bool operator==(const Vector3& ref) const;
    bool operator!=(const Vector3& ref) const;

    friend Vector3 operator*(IN const float scalar, IN const Vector3& ref);

    Vector3 Normalize() const;
    float   SquaredLength() const;
    float   Length() const;
    float   AngleSimilarity(const Vector3& ref) const;
    float   Angle(const Vector3& ref);
    bool    IsOrthogonal(const Vector3& ref);
    bool    IsNormal(const Vector3& ref);
};

} // namespace sbl
#endif
