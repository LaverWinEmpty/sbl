#include "../include/quaternion.h"
#include "../include/vector4.h"
#include "../include/matrix33.h"

namespace sbl {

Quaternion::Quaternion(): w(0), x(0), y(0), z(0) {}

Quaternion::Quaternion(IN const Quaternion& ref): w(ref.w), x(ref.x), y(ref.y), z(ref.z) {}

Quaternion::Quaternion(IN float w, IN float x, IN float y, IN float z): w(w), x(x), y(y), z(z) {}

Quaternion& Quaternion::operator=(IN const Quaternion& ref)
{
    w = ref.w;
    x = ref.x;
    y = ref.y;
    z = ref.z;
    return *this;
}

Quaternion Quaternion::operator*(IN const Quaternion& ref) const
{
    return Quaternion(w * ref.w - x * ref.x - y * ref.y - z * ref.z,
                      w * ref.x + x * ref.w + y * ref.z - z * ref.y,
                      w * ref.y - x * ref.z + y * ref.w + z * ref.x,
                      w * ref.z + x * ref.y - y * ref.x + z * ref.w);
}

Quaternion& Quaternion::operator*=(IN const Quaternion& ref)
{
    *this = *this * ref;
    return *this;
}

Vector3 Quaternion::operator*(IN const Vector3& ref) const
{
    Quaternion vec(0, ref.x, ref.y, ref.z);

    Quaternion rot = (*this) * vec * Conjugate();

    return { rot.x, rot.y, rot.z };
}

Quaternion Quaternion::Normalize() const
{
    float len = sqrtf(x * x + y * y + z * z + w * w);

    return { w / len, x / len, y / len, z / len };
}

Quaternion Quaternion::Conjugate() const
{
    return Quaternion(w, -x, -y, -z);
}

Matrix33 Quaternion::ToMatrix() const
{
    Matrix33 mat;

    mat[0][0] = 1.0f - 2.0f * (y * y + z * z);
    mat[0][1] = 2.0f * (x * y - w * z);
    mat[0][2] = 2.0f * (x * z + w * y);

    mat[1][0] = 2.0f * (x * y + w * z);
    mat[1][1] = 1.0f - 2.0f * (x * x + z * z);
    mat[1][2] = 2.0f * (y * z - w * x);

    mat[2][0] = 2.0f * (x * z - w * y);
    mat[2][1] = 2.0f * (y * z + w * x);
    mat[2][2] = 1.0f - 2.0f * (x * x + y * y);

    return mat;
}

Vector3 Quaternion::ToEuler()
{
    Vector3 v;

    float srcp = 2.f * (w * x + y * z);       // sin roll cos pitch
    float crcp = 1.f - 2.f * (x * x + y * y); // cos roll cos pitch
    v.x        = atan2f(srcp, crcp);          // roll

    float sp = 2.f * (w * y - z * x);                                 // sin pitch
    v.y      = ABS(sp) >= 1.f ? copysignf(PI * 0.5f, sp) : asinf(sp); // pitch

    float sycp = 2.f * (w * z + x * y);       // sin yaw cos pitch
    float cycp = 1.f - 2.f * (y * y + z * z); // cos yaw cos pitch
    v.z        = atan2f(sycp, cycp);          // yaw

    return RADIAN_TO_DEGREE(v);
}

// Degree
Quaternion Quaternion::Euler(IN float r, IN float p, IN float y)
{
    float roll  = DEGREE_TO_RADIAN(-r) * 0.5f;
    float pitch = DEGREE_TO_RADIAN(-p) * 0.5f;
    float yaw   = DEGREE_TO_RADIAN(-y) * 0.5f;

    float cy = cosf(yaw);
    float cp = cosf(pitch);
    float cr = cosf(roll);
    float sy = sinf(yaw);
    float sp = sinf(pitch);
    float sr = sinf(roll);

    float qw = cr * cp * cy + sr * sp * sy;
    float qx = sr * cp * cy - cr * sp * sy;
    float qy = cr * sp * cy + sr * cp * sy;
    float qz = cr * cp * sy - sr * sp * cy;

    return Quaternion{ qw, qx, qy, qz };
}

Quaternion Quaternion::FromAxisAngle(IN const Vector3& axis, IN float angle)
{
    angle = DEGREE_TO_RADIAN(angle) * 0.5f;

    float   s = sinf(angle);
    Vector3 n = axis.Normalize() * s;

    return { cosf(angle), n.x, n.y, n.z };
}

} // namespace sbl
