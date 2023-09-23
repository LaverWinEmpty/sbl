#include "../include/vector3.h"
#include "../include/vector4.h"
#include "../include/matrix33.h"
#include "../include/matrix44.h"
#include "../include/quaternion.h"

namespace sbl {

Vector3::Vector3(): Vector2(), z(0) {}

Vector3::Vector3(IN const float x, IN const float y, IN const float z): Vector2(x, y), z(z) {}

Vector3::Vector3(IN const Vector3& ref): Vector2(ref.x, ref.y), z(ref.z) {}

Vector3::Vector3(IN const Vector2& ref): Vector2(ref), z(1) {}

Vector3& Vector3::operator=(IN const Vector3& ref)
{
    x = ref.x;
    y = ref.y;
    z = ref.z;
    return *this;
}

Vector3 Vector3::operator+(IN const Vector3& ref) const
{
    return { x + ref.x, y + ref.y, z + ref.z };
}

Vector3 Vector3::operator-(IN const Vector3& ref) const
{
    return { x - ref.x, y - ref.y, z - ref.z };
}

Vector3 Vector3::operator*(IN const float scalar) const
{
    return { x * scalar, y * scalar, z * scalar };
}

Vector3 Vector3::operator/(IN const float scalar) const
{
    return { x / scalar, y / scalar, z / scalar };
}

float Vector3::operator*(IN const Vector3& ref) const
{
    return Vector2::operator*(ref) + z * ref.z;
}

Vector3& Vector3::operator+=(IN const Vector3& ref)
{
    *this = *this + ref;
    return *this;
}

Vector3& Vector3::operator-=(IN const Vector3& ref)
{
    *this = *this - ref;
    return *this;
}

Vector3& Vector3::operator*=(IN const float scalar)
{
    *this = *this * scalar;
    return *this;
}

Vector3& Vector3::operator/=(IN const float scalar)
{
    *this = *this / scalar;
    return *this;
}

Vector3 Vector3::operator^(IN const Vector3& ref) const
{
    return { y * ref.z - z * ref.y, z * ref.x - x * ref.z, x + ref.y - y * ref.x };
}

Vector3& Vector3::operator^=(IN const Vector3& ref)
{
    *this = *this ^ ref;
    return *this;
}

Vector3 Vector3::operator*(IN const Matrix33& ref) const
{
    Vector3 vec;
    vec.x = x * ref.m._00 + y * ref.m._10 + z * ref.m._20;
    vec.y = x * ref.m._01 + y * ref.m._11 + z * ref.m._21;
    vec.z = x * ref.m._02 + y * ref.m._12 + z * ref.m._22;
    return vec;
}

Vector4 Vector3::operator*(IN const Matrix44& ref) const
{
    Vector4 temp = *this;
    return temp * ref;
}

Vector3 Vector3::operator*(IN const Quaternion& ref) const
{
    return ref * *this;
}

Vector3& Vector3::operator*=(IN const Matrix33& ref)
{
    *this = *this * ref;
    return *this;
}

Vector3& Vector3::operator*=(IN const Matrix44& ref)
{
    *this = *this * ref;
    return *this;
}

Vector3& Vector3::operator*=(IN const Quaternion& ref)
{
    *this = ref * *this;
    return *this;
}

Vector3 Vector3::operator-() const
{
    return Vector3{ -x, -y, -z };
}

bool Vector3::operator==(IN const Vector3& ref) const
{
    return Vector2::operator==(ref) && FLOAT_EQUAL(z, ref.z);
}

bool Vector3::operator!=(IN const Vector3& ref) const
{
    return !operator==(ref);
}

Vector3 operator*(IN const float scalar, IN const Vector3& ref)
{
    return ref * scalar;
}

Vector3 Vector3::Normalize() const
{
    float length = Length();

    Vector3 temp;
    temp.x = x / length;
    temp.y = y / length;
    temp.z = z / length;
    return temp;
}

float Vector3::SquaredLength() const
{
    return Vector2::SquaredLength() + z * z;
}

float Vector3::Length() const
{
    return sqrtf(SquaredLength());
}

float Vector3::AngleSimilarity(IN const Vector3& ref) const
{
    return *this * ref / (Length() * ref.Length());
}

float Vector3::Angle(IN const Vector3& ref)
{
    return RADIAN_TO_DEGREE(acosf(AngleSimilarity(ref)));
}

bool Vector3::IsOrthogonal(IN const Vector3& ref)
{
    return AngleSimilarity(ref) <= EPSILON;
}

bool Vector3::IsNormal(IN const Vector3& ref)
{
    return ABS(*this * ref) < EPSILON;
}

} // namespace sbl
