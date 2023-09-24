#ifndef SBL_VECTOR_2D_HEADER__
#define SBL_VECTOR_2D_HEADER__

#include "gfxMath.h"

namespace sbl {

struct Vector2
{
    float x, y;

    Vector2();
    Vector2(IN const float x, IN const float y);
    Vector2(IN const Vector2& ref);

    Vector2& operator=(IN const Vector2& ref);

    Vector2 operator+(IN const Vector2& ref) const;
    Vector2 operator-(IN const Vector2& ref) const;
    Vector2 operator*(IN const float scalar) const;
    Vector2 operator/(IN const float scalar) const;
    float   operator*(IN const Vector2& ref) const;

    Vector2& operator+=(IN const Vector2& ref);
    Vector2& operator-=(IN const Vector2& ref);
    Vector2& operator*=(IN const float scalar);
    Vector2& operator/=(IN const float scalar);

    Vector3  operator*(IN const Matrix33& ref) const;
    Vector4  operator*(IN const Matrix44& ref) const;
    Vector2& operator*=(IN const Matrix33& ref);
    Vector2& operator*=(IN const Matrix44& ref);

    Vector2 operator-() const;

    bool operator==(IN const Vector2& ref) const;
    bool operator!=(IN const Vector2& ref) const;

    Vector2 Normalize() const;
    float   SquaredLength() const;
    float   Length() const;
    float   AngleSimilarity(IN const Vector2& ref) const;
    float   Angle(IN const Vector2& ref);
    bool    IsOrthogonal(IN const Vector2& ref);
    bool    IsNormal(IN const Vector2& ref);
};

} // namespace sbl
#endif