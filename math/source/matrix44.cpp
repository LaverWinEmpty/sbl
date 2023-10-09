#include "../include/matrix44.h"
#include "../include/matrix33.h"
#include "../include/vector4.h"
#include "../include/quaternion.h"

namespace sbl {

Matrix44::Matrix44(IN const float m00, IN const float m01, IN const float m02, IN const float m03, IN const float m10,
                   IN const float m11, IN const float m12, IN const float m13, IN const float m20, IN const float m21,
                   IN const float m22, IN const float m23, IN const float m30, IN const float m31, IN const float m32,
                   IN const float m33)
{
    m = { m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33 };
}

Matrix44::Matrix44()
{
    m = { 0 };
}

Matrix44::Matrix44(IN const Elements& param)
{
    m = param;
}

Matrix44::Matrix44(IN const Matrix44& ref)
{
    m = ref.m;
}

Matrix44::Matrix44(IN const Matrix33& ref)
{
    m._00 = ref.m._00;
    m._01 = ref.m._01;
    m._02 = ref.m._02;
    m._03 = 0;
    m._10 = ref.m._10;
    m._11 = ref.m._11;
    m._12 = ref.m._12;
    m._13 = 0;
    m._20 = ref.m._20;
    m._21 = ref.m._21;
    m._22 = ref.m._22;
    m._23 = 0;
    m._30 = 0;
    m._31 = 0;
    m._32 = 0;
    m._33 = 0;
}

float* Matrix44::operator[](size_t index)
{
    return row[index].col;
}

Matrix44& Matrix44::operator=(IN const Matrix44& ref)
{
    m = ref.m;
    return *this;
}

Matrix44& Matrix44::operator=(IN const Matrix33& ref)
{
    m._00 = ref.m._00;
    m._01 = ref.m._01;
    m._02 = ref.m._02;
    m._03 = 0;
    m._10 = ref.m._10;
    m._11 = ref.m._11;
    m._12 = ref.m._12;
    m._13 = 0;
    m._20 = ref.m._20;
    m._21 = ref.m._21;
    m._22 = ref.m._22;
    m._23 = 0;
    m._30 = 0;
    m._31 = 0;
    m._32 = 0;
    m._33 = 0;
    return *this;
}

// Matrix44& Matrix44::operator=(IN const Elements& param)
//{
//     m = param;
//     return *this;
// }

Matrix44 Matrix44::operator+(IN const Matrix44& ref) const
{
    return { m._00 + ref.m._00, m._01 + ref.m._01, m._02 + ref.m._02, m._03 + ref.m._03,
             m._10 + ref.m._10, m._11 + ref.m._11, m._12 + ref.m._12, m._13 + ref.m._13,
             m._20 + ref.m._20, m._21 + ref.m._21, m._22 + ref.m._22, m._23 + ref.m._23,
             m._30 + ref.m._30, m._31 + ref.m._31, m._32 + ref.m._32, m._33 + ref.m._33 };
}

Matrix44 Matrix44::operator-(IN const Matrix44& ref) const
{
    return { m._00 - ref.m._00, m._01 - ref.m._01, m._02 + ref.m._02, m._03 - ref.m._03,
             m._10 - ref.m._10, m._11 + ref.m._11, m._12 - ref.m._12, m._13 - ref.m._13,
             m._20 + ref.m._20, m._21 - ref.m._21, m._22 - ref.m._22, m._23 + ref.m._23,
             m._30 - ref.m._30, m._31 - ref.m._31, m._32 + ref.m._32, m._33 - ref.m._33 };
}

Matrix44 Matrix44::operator*(IN const Matrix44& ref) const
{
    Matrix44 mat;
    mat[0][0] = m._00 * ref.m._00 + m._01 * ref.m._10 + m._02 * ref.m._20 + m._03 * ref.m._30;
    mat[0][1] = m._00 * ref.m._01 + m._01 * ref.m._11 + m._02 * ref.m._21 + m._03 * ref.m._31;
    mat[0][2] = m._00 * ref.m._02 + m._01 * ref.m._12 + m._02 * ref.m._22 + m._03 * ref.m._32;
    mat[0][3] = m._00 * ref.m._03 + m._01 * ref.m._13 + m._02 * ref.m._23 + m._03 * ref.m._33;
    mat[1][0] = m._10 * ref.m._00 + m._11 * ref.m._10 + m._12 * ref.m._20 + m._13 * ref.m._30;
    mat[1][1] = m._10 * ref.m._01 + m._11 * ref.m._11 + m._12 * ref.m._21 + m._13 * ref.m._31;
    mat[1][2] = m._10 * ref.m._02 + m._11 * ref.m._12 + m._12 * ref.m._22 + m._13 * ref.m._32;
    mat[1][3] = m._10 * ref.m._03 + m._11 * ref.m._13 + m._12 * ref.m._23 + m._13 * ref.m._33;
    mat[2][0] = m._20 * ref.m._00 + m._21 * ref.m._10 + m._22 * ref.m._20 + m._23 * ref.m._30;
    mat[2][1] = m._20 * ref.m._01 + m._21 * ref.m._11 + m._22 * ref.m._21 + m._23 * ref.m._31;
    mat[2][2] = m._20 * ref.m._02 + m._21 * ref.m._12 + m._22 * ref.m._22 + m._23 * ref.m._32;
    mat[2][3] = m._20 * ref.m._03 + m._21 * ref.m._13 + m._22 * ref.m._23 + m._23 * ref.m._33;
    mat[3][0] = m._30 * ref.m._00 + m._31 * ref.m._10 + m._32 * ref.m._20 + m._33 * ref.m._30;
    mat[3][1] = m._30 * ref.m._01 + m._31 * ref.m._11 + m._32 * ref.m._21 + m._33 * ref.m._31;
    mat[3][2] = m._30 * ref.m._02 + m._31 * ref.m._12 + m._32 * ref.m._22 + m._33 * ref.m._32;
    mat[3][3] = m._30 * ref.m._03 + m._31 * ref.m._13 + m._32 * ref.m._23 + m._33 * ref.m._33;
    return mat;
}

Matrix44 Matrix44::operator*(IN const float scalar) const
{
    return { m._00 * scalar, m._01 * scalar, m._02 * scalar, m._03 * scalar, m._10 * scalar, m._11 * scalar,
             m._12 * scalar, m._13 * scalar, m._20 * scalar, m._21 * scalar, m._22 * scalar, m._23 * scalar,
             m._30 * scalar, m._31 * scalar, m._32 * scalar, m._33 * scalar };
}

Matrix44 Matrix44::operator/(IN const float scalar) const
{
    return { m._00 / scalar, m._01 / scalar, m._02 / scalar, m._03 / scalar, m._10 / scalar, m._11 / scalar,
             m._12 / scalar, m._13 / scalar, m._20 / scalar, m._21 / scalar, m._22 / scalar, m._23 / scalar,
             m._30 / scalar, m._31 / scalar, m._32 / scalar, m._33 / scalar };
}

Vector4 Matrix44::operator*(IN const Vector4& ref) const
{
    Vector4 vec;
    vec.x = ref.x * m._00 + ref.y * m._01 + ref.z * m._02 + ref.w * m._03;
    vec.y = ref.x * m._10 + ref.y * m._11 + ref.z * m._12 + ref.w * m._13;
    vec.z = ref.x * m._20 + ref.y * m._21 + ref.z * m._22 + ref.w * m._23;
    vec.w = ref.x * m._30 + ref.y * m._31 + ref.z * m._32 + ref.w * m._33;
    return vec;
}

Matrix44& Matrix44::operator+=(IN const Matrix44& ref)
{
    *this = *this + ref;
    return *this;
}

Matrix44& Matrix44::operator-=(IN const Matrix44& ref)
{
    *this = *this - ref;
    return *this;
}

Matrix44& Matrix44::operator*=(IN const Matrix44& ref)
{
    *this = *this * ref;
    return *this;
}

Matrix44& Matrix44::operator*=(IN const float scalar)
{
    *this = *this * scalar;
    return *this;
}

Matrix44& Matrix44::operator/=(IN const float scalar)
{
    *this = *this / scalar;
    return *this;
}

bool Matrix44::operator==(IN const Matrix44& ref) const
{
    return FLOAT_EQUAL(m._00, ref.m._00) && FLOAT_EQUAL(m._01, ref.m._01) && FLOAT_EQUAL(m._02, ref.m._02) &&
           FLOAT_EQUAL(m._03, ref.m._03) && FLOAT_EQUAL(m._10, ref.m._10) && FLOAT_EQUAL(m._11, ref.m._11) &&
           FLOAT_EQUAL(m._12, ref.m._12) && FLOAT_EQUAL(m._13, ref.m._13) && FLOAT_EQUAL(m._20, ref.m._20) &&
           FLOAT_EQUAL(m._21, ref.m._21) && FLOAT_EQUAL(m._22, ref.m._22) && FLOAT_EQUAL(m._23, ref.m._23) &&
           FLOAT_EQUAL(m._30, ref.m._30) && FLOAT_EQUAL(m._31, ref.m._31) && FLOAT_EQUAL(m._32, ref.m._32) &&
           FLOAT_EQUAL(m._33, ref.m._33);
}

bool Matrix44::operator!=(IN const Matrix44& ref) const
{
    return !operator==(ref);
}

Matrix44 Matrix44::Zero()
{
    return { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
}

Matrix44 Matrix44::Identity()
{
    return { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}

Matrix44 Matrix44::Translation(IN const float x, IN const float y, IN const float z)
{
    return { 1, 0, 0, x, 0, 1, 0, y, 0, 0, 1, z, x, y, z, 1 };
}

Matrix44 Matrix44::Scale(IN const float x, IN const float y, IN const float z, IN const float w)
{
    return { x, 0, 0, 0, 0, y, 0, 0, 0, 0, z, 0, 0, 0, 0, w };
}

Matrix44 Matrix44::RotationX(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);

    return { 1, 0, 0, 0, 0, c, -s, 0, 0, s, c, 0, 0, 0, 0, 1 };
}

Matrix44 Matrix44::RotationY(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);
    return { c, 0, s, 0, 0, 1, 0, 0, -s, 0, c, 0, 0, 0, 0, 1 };
}

Matrix44 Matrix44::RotationZ(IN const float angle)
{
    float theta = DEGREE_TO_RADIAN(angle);
    float s     = sinf(theta);
    float c     = cosf(theta);
    return { c, -s, 0, 0, s, c, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
}

Matrix44 Matrix44::Transpose()
{
    Matrix44 temp = *this;
    SWAP(temp.m._01, temp.m._10);
    SWAP(temp.m._02, temp.m._20);
    SWAP(temp.m._03, temp.m._30);
    SWAP(temp.m._12, temp.m._21);
    SWAP(temp.m._13, temp.m._31);
    SWAP(temp.m._23, temp.m._32);
    return temp;
}

// throw
// runtime_error
Matrix44 Matrix44::Inverse()
{
    float det = Determinant();
    if(FLOAT_EQUAL(det, 0)) {
        throw std::runtime_error("DETERMINANT ZERO");
    }
    float detInv = 1 / det;

    Matrix44 temp;

    // Row 1
    temp[0][0] = Matrix33(m._11, m._12, m._13, m._21, m._22, m._23, m._31, m._32, m._33).Determinant() * detInv;
    temp[0][1] = Matrix33(m._10, m._12, m._13, m._20, m._22, m._23, m._30, m._32, m._33).Determinant() * detInv * -1;
    temp[0][2] = Matrix33(m._10, m._11, m._13, m._20, m._21, m._23, m._30, m._31, m._33).Determinant() * detInv;
    temp[0][3] = Matrix33(m._10, m._11, m._12, m._20, m._21, m._22, m._30, m._31, m._32).Determinant() * detInv * -1;

    // Row 2
    temp[1][0] = Matrix33(m._01, m._02, m._03, m._21, m._22, m._23, m._31, m._32, m._33).Determinant() * detInv * -1;
    temp[1][1] = Matrix33(m._00, m._02, m._03, m._20, m._22, m._23, m._30, m._32, m._33).Determinant() * detInv;
    temp[1][2] = Matrix33(m._00, m._01, m._03, m._20, m._21, m._23, m._30, m._31, m._33).Determinant() * detInv * -1;
    temp[1][3] = Matrix33(m._00, m._01, m._02, m._20, m._21, m._22, m._30, m._31, m._32).Determinant() * detInv;

    // Row 3
    temp[2][0] = Matrix33(m._01, m._02, m._03, m._11, m._12, m._13, m._31, m._32, m._33).Determinant() * detInv;
    temp[2][1] = Matrix33(m._00, m._02, m._03, m._10, m._12, m._13, m._30, m._32, m._33).Determinant() * detInv * -1;
    temp[2][2] = Matrix33(m._00, m._01, m._03, m._10, m._11, m._13, m._30, m._31, m._33).Determinant() * detInv;
    temp[2][3] = Matrix33(m._00, m._01, m._02, m._10, m._11, m._12, m._30, m._31, m._32).Determinant() * detInv * -1;

    // Row 4
    temp[3][0] = Matrix33(m._01, m._02, m._03, m._11, m._12, m._13, m._21, m._22, m._23).Determinant() * detInv * -1;
    temp[3][1] = Matrix33(m._00, m._02, m._03, m._10, m._12, m._13, m._20, m._22, m._23).Determinant() * detInv;
    temp[3][2] = Matrix33(m._00, m._01, m._03, m._10, m._11, m._13, m._20, m._21, m._23).Determinant() * detInv * -1;
    temp[3][3] = Matrix33(m._00, m._01, m._02, m._10, m._11, m._12, m._20, m._21, m._22).Determinant() * detInv;

    return temp;
}

float Matrix44::Determinant()
{
    float det1, det2, det3, det4;
    det1 = Matrix33{ m._11, m._12, m._13, m._21, m._22, m._23, m._31, m._32, m._33 }.Determinant();
    det2 = Matrix33{ m._10, m._12, m._13, m._20, m._22, m._23, m._30, m._32, m._33 }.Determinant();
    det3 = Matrix33{ m._10, m._11, m._13, m._20, m._21, m._23, m._30, m._31, m._33 }.Determinant();
    det4 = Matrix33{ m._10, m._11, m._12, m._20, m._21, m._22, m._30, m._31, m._32 }.Determinant();
    return m._00 * det1 - m._01 * det2 + m._02 * det3 - m._03 * det4;
}

bool Matrix44::IsSymmetric()
{
    return FLOAT_EQUAL(m._01, m._10) && FLOAT_EQUAL(m._02, m._20) && FLOAT_EQUAL(m._03, m._30) &&
           FLOAT_EQUAL(m._12, m._21) && FLOAT_EQUAL(m._13, m._31) && FLOAT_EQUAL(m._23, m._32);
}

bool Matrix44::IsInvertible()
{
    return !FLOAT_EQUAL(Determinant(), 0);
}

} // namespace sbl