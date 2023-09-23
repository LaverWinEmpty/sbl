#include "../include/matrix33.h"
#include "../include/vector4.h"
#include "../include/quaternion.h"

namespace sbl {

Matrix33::Matrix33(IN const float m00, IN const float m01, IN const float m02, IN const float m10, IN const float m11,
                   IN const float m12, IN const float m20, IN const float m21, IN const float m22)
{
    m = { m00, m01, m02, m10, m11, m12, m20, m21, m22 };
}

Matrix33::Matrix33()
{
    m = { 0 };
}

Matrix33::Matrix33(IN const Elements& param)
{
    m = param;
}

Matrix33::Matrix33(IN const Matrix33& ref)
{
    m = ref.m;
}

float* Matrix33::operator[](size_t index)
{
    return row[index].col;
}

Matrix33& Matrix33::operator=(IN const Matrix33& ref)
{
    m = ref.m;
    return *this;
}

Matrix33 Matrix33::operator+(IN const Matrix33& ref) const
{
    return { m._00 + ref.m._00, m._01 + ref.m._01, m._02 + ref.m._02, m._10 + ref.m._10, m._11 + ref.m._11,
             m._12 + ref.m._12, m._20 + ref.m._20, m._21 + ref.m._21, m._22 + ref.m._22 };
}

Matrix33 Matrix33::operator-(IN const Matrix33& ref) const
{
    return { m._00 - ref.m._00, m._01 - ref.m._01, m._02 - ref.m._02, m._10 - ref.m._10, m._11 - ref.m._11,
             m._12 - ref.m._12, m._20 - ref.m._20, m._21 - ref.m._21, m._22 - ref.m._22 };
}

Matrix33 Matrix33::operator*(IN const Matrix33& ref) const
{
    Matrix33 mat;
    mat[0][0] = m._00 * ref.m._00 + m._01 * ref.m._10 + m._02 * ref.m._20;
    mat[0][1] = m._00 * ref.m._01 + m._01 * ref.m._11 + m._02 * ref.m._21;
    mat[0][2] = m._00 * ref.m._02 + m._01 * ref.m._12 + m._02 * ref.m._22;
    mat[1][0] = m._10 * ref.m._00 + m._11 * ref.m._10 + m._12 * ref.m._20;
    mat[1][1] = m._10 * ref.m._01 + m._11 * ref.m._11 + m._12 * ref.m._21;
    mat[1][2] = m._10 * ref.m._02 + m._11 * ref.m._12 + m._12 * ref.m._22;
    mat[2][0] = m._20 * ref.m._00 + m._21 * ref.m._10 + m._22 * ref.m._20;
    mat[2][1] = m._20 * ref.m._01 + m._21 * ref.m._11 + m._22 * ref.m._21;
    mat[2][2] = m._20 * ref.m._02 + m._21 * ref.m._12 + m._22 * ref.m._22;
    return mat;
}

Matrix33 Matrix33::operator*(IN const float scalar) const
{
    return { m._00 * scalar, m._01 * scalar, m._02 * scalar, m._10 * scalar, m._11 * scalar,
             m._12 * scalar, m._20 * scalar, m._21 * scalar, m._22 * scalar };
}

Matrix33 Matrix33::operator/(IN const float scalar) const
{
    return { m._00 / scalar, m._01 / scalar, m._02 / scalar, m._10 * scalar, m._11 / scalar,
             m._12 / scalar, m._20 / scalar, m._21 / scalar, m._22 * scalar };
}

Vector3 Matrix33::operator*(IN const Vector3& ref) const
{
    Vector3 vec;
    vec.x = ref.x * m._00 + ref.y * m._01 + ref.z * m._02;
    vec.y = ref.x * m._10 + ref.y * m._11 + ref.z * m._12;
    vec.z = ref.x * m._20 + ref.y * m._21 + ref.z * m._22;
    return vec;
}

Matrix33& Matrix33::operator+=(IN const Matrix33& ref)
{
    *this = *this + ref;
    return *this;
}

Matrix33& Matrix33::operator-=(IN const Matrix33& ref)
{
    *this = *this - ref;
    return *this;
}

Matrix33& Matrix33::operator*=(IN const Matrix33& ref)
{
    *this = *this * ref;
    return *this;
}

Matrix33& Matrix33::operator*=(IN const float scalar)
{
    *this = *this * scalar;
    return *this;
}

Matrix33& Matrix33::operator/=(IN const float scalar)
{
    *this = *this / scalar;
    return *this;
}

bool Matrix33::operator==(IN const Matrix33& ref) const
{
    return FLOAT_EQUAL(m._00, ref.m._00) && FLOAT_EQUAL(m._01, ref.m._01) && FLOAT_EQUAL(m._02, ref.m._02) &&
           FLOAT_EQUAL(m._10, ref.m._10) && FLOAT_EQUAL(m._11, ref.m._11) && FLOAT_EQUAL(m._12, ref.m._12) &&
           FLOAT_EQUAL(m._20, ref.m._20) && FLOAT_EQUAL(m._21, ref.m._21) && FLOAT_EQUAL(m._22, ref.m._22);
}

bool Matrix33::operator!=(IN const Matrix33& ref) const
{
    return !operator==(ref);
}

Matrix33 Matrix33::Zero()
{
    return { 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

Matrix33 Matrix33::Identity()
{
    return { 1.f, 0, 0, 0, 1.f, 0, 0, 0, 1.f };
}

Matrix33 Matrix33::Translation(IN const float x, IN const float y)
{
    return { 1, 0, x, 0, 1, y, x, y, 1 };
}

Matrix33 Matrix33::Scale(IN const float x, IN const float y, IN const float z)
{
    return { x, 0, 0, 0, y, 0, 0, 0, z };
}

Matrix33 Matrix33::RotationX(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);
    return { 1, 0, 0, 0, c, -s, 0, s, c };
}

Matrix33 Matrix33::RotationY(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);
    return { c, 0, s, 0, 1, 0, -s, 0, c };
}

Matrix33 Matrix33::RotationZ(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);
    return { c, -s, 0, s, c, 0, 0, 0, 1 };
}

Matrix33 Matrix33::Transpose()
{
    Matrix33 temp = *this;
    SWAP(temp.m._01, temp.m._10);
    SWAP(temp.m._02, temp.m._20);
    SWAP(temp.m._12, temp.m._21);
    return temp;
}

// Throw MsgErr
Matrix33 Matrix33::Inverse()
{
    float det = Determinant();
    if(FLOAT_EQUAL(det, 0)) {
        throw ErrorBuilder::InvalidValue();
    }

    float detInv = 1 / det;

    Matrix33 temp;
    // Row 1
    temp.m._00 = (m._11 * m._22 - m._12 * m._21) * detInv;
    temp.m._01 = (m._10 * m._22 - m._12 * m._20) * detInv * -1;
    temp.m._02 = (m._10 * m._21 - m._11 * m._20) * detInv;

    // Row 2
    temp.m._10 = (m._01 * m._22 - m._02 * m._21) * detInv * -1;
    temp.m._11 = (m._00 * m._22 - m._02 * m._20) * detInv;
    temp.m._12 = (m._00 * m._21 - m._01 * m._20) * detInv * -1;

    // Row 3
    temp.m._20 = (m._01 * m._12 - m._02 * m._11) * detInv;
    temp.m._21 = (m._00 * m._12 - m._02 * m._10) * detInv * -1;
    temp.m._22 = (m._00 * m._11 - m._01 * m._10) * detInv;

    return temp;
}

float Matrix33::Determinant()
{
    return m._00 * (m._11 * m._22 - m._12 * m._21) - m._01 * (m._10 * m._22 - m._12 * m._20) +
           m._02 * (m._10 * m._21 - m._11 * m._20);
}

bool Matrix33::IsSymmetric()
{
    return FLOAT_EQUAL(m._01, m._10) && FLOAT_EQUAL(m._02, m._20) && FLOAT_EQUAL(m._12, m._21);
}

bool Matrix33::IsInvertible()
{
    return FLOAT_EQUAL(Determinant(), 0) == false;
}

} // namespace sbl