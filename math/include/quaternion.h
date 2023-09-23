#ifndef __SEMIBASE_QUATERNION_HEADER__
#define __SEMIBASE_QUATERNION_HEADER__

#include "gfxMath.h"

namespace sbl {

struct Quaternion
{
public:
    Quaternion();
    Quaternion(IN const Quaternion& ref);
    Quaternion(IN float w, IN float x, IN float y, IN float z);

public:
    Quaternion& operator=(IN const Quaternion& ref);
    Quaternion  operator*(IN const Quaternion& ref) const;
    Quaternion& operator*=(IN const Quaternion& ref);
    Vector3     operator*(IN const Vector3& ref) const;

public:
    Quaternion Normalize() const;
    Quaternion Conjugate() const;

public:
    Matrix33 ToMatrix() const;
    Vector3  ToEuler();

public:
    static Quaternion Euler(IN float roll, IN float pitch, IN float yaw);
    static Quaternion FromAxisAngle(IN const Vector3& axis, IN float angle);

public:
    float w, x, y, z;
};

} // namespace sbl
#endif