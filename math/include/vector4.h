#ifndef __SEMIBASE_VECTOR_4_HEADER__
#define __SEMIBASE_VECTOR_4_HEADER__

#include "vector3.h"

namespace sbl {

struct Vector4: Vector3
{
    float w;

    Vector4();
    Vector4(IN const float x, IN const float y, IN const float z, IN const float w);
    Vector4(IN const Vector4& ref);
    Vector4(IN const Vector3& ref);
    Vector4(IN const Vector2& ref);

    Vector4& operator=(IN const Vector4& ref);

    Vector4 operator+(IN const Vector4& ref) const;
    Vector4 operator-(IN const Vector4& ref) const;
    Vector4 operator*(IN const float scalar) const;
    Vector4 operator/(IN const float scalar) const;
    float   operator*(IN const Vector4& ref) const;

    Vector4& operator+=(IN const Vector4& ref);
    Vector4& operator-=(IN const Vector4& ref);
    Vector4& operator*=(IN const float scalar);
    Vector4& operator/=(IN const float scalar);

    Vector4  operator*(IN const Matrix44& ref) const;
    Vector4& operator*=(IN const Matrix44& ref);

    Vector4 operator-() const;

    bool operator==(IN const Vector4& ref) const;
    bool operator!=(IN const Vector4& ref) const;

    Vector4 Normalize() const;
    float   SquaredLength() const;
    float   Length() const;
    float   AngleSimilarity(const Vector4& ref) const;
    float   Angle(IN const Vector4& ref);
    bool    IsOrthogonal(IN const Vector4& ref);
    bool    IsNormal(IN const Vector4& ref);
};

} // namespace sbl
#endif