#include "../include/vector2.h"
#include "../include/vector3.h"
#include "../include/vector4.h"
#include "../include/matrix33.h"
#include "../include/matrix44.h"

namespace sbl {

Vector2::Vector2(): x(0), y(0) {}

Vector2::Vector2(IN const float x, IN const float y): x(x), y(y) {}

Vector2::Vector2(IN const Vector2& ref): x(ref.x), y(ref.y) {}

Vector2& Vector2::operator=(IN const Vector2& ref)
{
    x = ref.x;
    y = ref.y;
    return *this;
}

Vector2 Vector2::operator+(IN const Vector2& ref) const
{
    return { x + ref.x, y + ref.y };
}

Vector2 Vector2::operator-(IN const Vector2& ref) const
{
    return { x - ref.x, y - ref.y };
}

Vector2 Vector2::operator*(IN const float scalar) const
{
    return { x * scalar, y * scalar };
}

Vector2 Vector2::operator/(IN const float scalar) const
{
    return { x / scalar, y / scalar };
}

float Vector2::operator*(IN const Vector2& ref) const
{
    return x * ref.x + y * ref.y;
}

Vector2& Vector2::operator+=(IN const Vector2& ref)
{
    *this = *this + ref;
    return *this;
}

Vector2& Vector2::operator-=(IN const Vector2& ref)
{
    *this = *this - ref;
    return *this;
}

Vector2& Vector2::operator*=(IN const float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector2& Vector2::operator/=(IN const float scalar)
{
    *this = *this / scalar;
    return *this;
}

Vector3 Vector2::operator*(IN const Matrix33& ref) const
{
    Vector3 temp = *this;
    return temp * ref;
}

Vector4 Vector2::operator*(IN const Matrix44& ref) const
{
    Vector4 temp = *this;
    return temp * ref;
}

Vector2& Vector2::operator*=(IN const Matrix33& ref)
{
    *this = *this * ref;
    return *this;
}

Vector2& Vector2::operator*=(IN const Matrix44& ref)
{
    *this = *this * ref;
    return *this;
}

Vector2 Vector2::operator-() const
{
    return Vector2{ -x, -y };
}

bool Vector2::operator==(IN const Vector2& ref) const
{
    return FLOAT_EQUAL(x, ref.x) && FLOAT_EQUAL(y, ref.y);
}

bool Vector2::operator!=(IN const Vector2& ref) const
{
    return !operator==(ref);
}

Vector2 Vector2::Normalize() const
{
    float   length = Length();
    Vector2 temp;
    temp.x = x / length;
    temp.y = y / length;
    return temp;
}

float Vector2::SquaredLength() const
{
    return x * x + y * y;
}

float Vector2::Length() const
{
    return sqrtf(SquaredLength());
}

float Vector2::AngleSimilarity(IN const Vector2& ref) const
{
    return *this * ref / (Length() * ref.Length());
}

float Vector2::Angle(IN const Vector2& ref)
{
    return RADIAN_TO_DEGREE(acosf(AngleSimilarity(ref)));
}

bool Vector2::IsOrthogonal(IN const Vector2& ref)
{
    return AngleSimilarity(ref) <= EPSILON;
}

bool Vector2::IsNormal(IN const Vector2& ref)
{
    return ABS(*this * ref) < EPSILON;
}

} // namespace sbl
