#include "../include/vector4.h"
#include "../include/matrix44.h"

namespace sbl {

Vector4::Vector4(): Vector3(), w(0) {}

Vector4::Vector4(IN const float x, IN const float y, IN const float z, IN const float w): Vector3(x, y, z), w(w) {}

Vector4::Vector4(IN const Vector4& ref): Vector3(ref.x, ref.y, ref.z), w(ref.w) {}

Vector4::Vector4(IN const Vector3& ref): Vector3(ref), w(1) {}

Vector4::Vector4(IN const Vector2& ref): Vector3(ref), w(1) {}

Vector4& Vector4::operator=(IN const Vector4& ref)
{
    x = ref.x;
    y = ref.y;
    z = ref.z;
    w = ref.w;
    return *this;
}

Vector4 Vector4::operator+(IN const Vector4& ref) const
{
    return { x + ref.x, y + ref.y, z + ref.z, w + ref.w };
}

Vector4 Vector4::operator-(IN const Vector4& ref) const
{
    return { x - ref.x, y - ref.y, z - ref.z, w - ref.w };
}

Vector4 Vector4::operator*(IN const float scalar) const
{
    return { x * scalar, y * scalar, z * scalar, w * scalar };
}

Vector4 Vector4::operator/(IN const float scalar) const
{
    return { x / scalar, y / scalar, z / scalar, w / scalar };
}

float Vector4::operator*(IN const Vector4& ref) const
{
    return Vector3::operator*(ref) + w * ref.w;
}

Vector4& Vector4::operator+=(IN const Vector4& ref)
{
    *this = *this + ref;
    return *this;
}

Vector4& Vector4::operator-=(IN const Vector4& ref)
{
    *this = *this - ref;
    return *this;
}

Vector4& Vector4::operator*=(IN const float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector4& Vector4::operator/=(IN const float scalar)
{
    *this = *this / scalar;
    return *this;
}

Vector4 Vector4::operator*(IN const Matrix44& ref) const
{
    Vector4 vec;

    vec.x = x * ref.m._00 + y * ref.m._10 + z * ref.m._20 + w * ref.m._30;
    vec.y = x * ref.m._01 + y * ref.m._11 + z * ref.m._21 + w * ref.m._31;
    vec.z = x * ref.m._02 + y * ref.m._12 + z * ref.m._22 + w * ref.m._32;
    vec.w = x * ref.m._03 + y * ref.m._13 + z * ref.m._23 + w * ref.m._33;

    return vec;
}

Vector4& Vector4::operator*=(IN const Matrix44& ref)
{
    *this = *this * ref;
    return *this;
}

Vector4 Vector4::operator-() const
{
    return Vector4{ -x, -y, -z, -w };
}

bool Vector4::operator==(IN const Vector4& ref) const
{
    return Vector3::operator==(ref) && FLOAT_EQUAL(w, ref.w);
}

bool Vector4::operator!=(IN const Vector4& ref) const
{
    return !operator==(ref);
}

Vector4 Vector4::Normalize() const
{
    float length = Length();

    Vector4 temp;
    temp.x = x / length;
    temp.y = y / length;
    temp.z = z / length;
    temp.w = w / length;
    return temp;
}

float Vector4::SquaredLength() const
{
    return Vector3::SquaredLength() + w * w;
}

float Vector4::Length() const
{
    return sqrtf(SquaredLength());
}

float Vector4::AngleSimilarity(IN const Vector4& ref) const
{
    return *this * ref / (Length() * ref.Length());
}

// Return: degree
float Vector4::Angle(IN const Vector4& ref)
{
    return RADIAN_TO_DEGREE(acosf(AngleSimilarity(ref)));
}

bool Vector4::IsOrthogonal(IN const Vector4& ref)
{
    return FLOAT_EQUAL(AngleSimilarity(ref), 0);
}

bool Vector4::IsNormal(IN const Vector4& ref)
{
    return ABS(*this * ref) < EPSILON;
}

} // namespace sbl
