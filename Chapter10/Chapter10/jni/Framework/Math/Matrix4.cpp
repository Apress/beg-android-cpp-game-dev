#include "Matrix4.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3.h"
#include <cmath>
#include <cassert>

namespace Framework
{

Matrix4::Matrix4()
{
}

Matrix4::~Matrix4()
{
}

void Matrix4::Identify()
{
	m_m[0] = 1.0f;
	m_m[1] = 0.0f;
	m_m[2] = 0.0f;
	m_m[3] = 0.0f;
	m_m[4] = 0.0f;
	m_m[5] = 1.0f;
	m_m[6] = 0.0f;
	m_m[7] = 0.0f;
	m_m[8] = 0.0f;
	m_m[9] = 0.0f;
	m_m[10] = 1.0f;
	m_m[11] = 0.0f;
	m_m[12] = 0.0f;
	m_m[13] = 0.0f;
	m_m[14] = 0.0f;
	m_m[15] = 1.0f;
}

Vector3 Matrix4::Transform(const Vector3& in) const
{
	return Vector3((m_m[0] * in.m_x) + (m_m[1] * in.m_y) + (m_m[2] * in.m_z),
			(m_m[4] * in.m_x) + (m_m[5] * in.m_y) + (m_m[6] * in.m_z),
			(m_m[6] * in.m_x) + (m_m[7] * in.m_y) + (m_m[8] * in.m_z));
}

Vector3 Matrix4::TransformTranspose(const Vector3& in) const
{
	return Vector3((m_m[0] * in.m_x) + (m_m[3] * in.m_y) + (m_m[6] * in.m_z),
			(m_m[1] * in.m_x) + (m_m[4] * in.m_y) + (m_m[7] * in.m_z),
			(m_m[2] * in.m_x) + (m_m[5] * in.m_y) + (m_m[8] * in.m_z));
}

Vector4 Matrix4::Multiply(const Vector4& in) const
{
	return Vector4((m_m[0] * in.m_x) + (m_m[1] * in.m_y) + (m_m[2] * in.m_z) + (m_m[3] * in.m_w),
			(m_m[4] * in.m_x) + (m_m[5] * in.m_y) + (m_m[6] * in.m_z) + (m_m[7] * in.m_w),
			(m_m[8] * in.m_x) + (m_m[9] * in.m_y) + (m_m[10] * in.m_z) + (m_m[11] * in.m_w),
			(m_m[12] * in.m_x) + (m_m[13] * in.m_y) + (m_m[14] * in.m_z) + (m_m[15] * in.m_w));
}

void Matrix4::RotateAroundX(float radians)
{
	m_m[0] = 1.0f; m_m[1] = 0.0f; m_m[2] = 0.0f;
	m_m[4] = 0.0f; m_m[5] = cos(radians); m_m[6] = sin(radians);
	m_m[8] = 0.0f; m_m[9] = -sin(radians); m_m[10] = cos(radians);
}

void Matrix4::RotateAroundY(float radians)
{
	m_m[0] = cos(radians); m_m[1] = 0.0f; m_m[2] = -sin(radians);
	m_m[4] = 0.0f; m_m[5] = 1.0f; m_m[6] = 0.0f;
	m_m[8] = sin(radians); m_m[9] = 0.0f; m_m[10] = cos(radians);
}

void Matrix4::RotateAroundZ(float radians)
{
	m_m[0] = cos(radians); m_m[1] = sin(radians); m_m[2] = 0.0f;
	m_m[4] = -sin(radians); m_m[5] = cos(radians); m_m[6] = 0.0f;
	m_m[8] = 0.0f; m_m[9] = 0.0f; m_m[10] = 1.0f;
}

void Matrix4::Multiply(const Matrix4& in, Matrix4& out) const
{
	assert(this != &in && this != &out && &in != &out);
	out.m_m[0] = (m_m[0] * in.m_m[0]) + (m_m[1] * in.m_m[4]) + (m_m[2] * in.m_m[8]) + (m_m[3] * in.m_m[12]);
	out.m_m[1] = (m_m[0] * in.m_m[1]) + (m_m[1] * in.m_m[5]) + (m_m[2] * in.m_m[9]) + (m_m[3] * in.m_m[13]);
	out.m_m[2] = (m_m[0] * in.m_m[2]) + (m_m[1] * in.m_m[6]) + (m_m[2] * in.m_m[10]) + (m_m[3] * in.m_m[14]);
	out.m_m[3] = (m_m[0] * in.m_m[3]) + (m_m[1] * in.m_m[7]) + (m_m[2] * in.m_m[11]) + (m_m[3] * in.m_m[15]);
	out.m_m[4] = (m_m[4] * in.m_m[0]) + (m_m[5] * in.m_m[4]) + (m_m[6] * in.m_m[8]) + (m_m[7] * in.m_m[12]);
	out.m_m[5] = (m_m[4] * in.m_m[1]) + (m_m[5] * in.m_m[5]) + (m_m[6] * in.m_m[9]) + (m_m[7] * in.m_m[13]);
	out.m_m[6] = (m_m[4] * in.m_m[2]) + (m_m[5] * in.m_m[6]) + (m_m[6] * in.m_m[10]) + (m_m[7] * in.m_m[14]);
	out.m_m[7] = (m_m[4] * in.m_m[3]) + (m_m[5] * in.m_m[7]) + (m_m[6] * in.m_m[11]) + (m_m[7] * in.m_m[15]);
	out.m_m[8] = (m_m[8] * in.m_m[0]) + (m_m[9] * in.m_m[4]) + (m_m[10] * in.m_m[8]) + (m_m[11] * in.m_m[12]);
	out.m_m[9] = (m_m[8] * in.m_m[1]) + (m_m[9] * in.m_m[5]) + (m_m[10] * in.m_m[9]) + (m_m[11] * in.m_m[13]);
	out.m_m[10] = (m_m[8] * in.m_m[2]) + (m_m[9] * in.m_m[6]) + (m_m[10] * in.m_m[10]) + (m_m[11] * in.m_m[14]);
	out.m_m[11] = (m_m[8] * in.m_m[3]) + (m_m[9] * in.m_m[7]) + (m_m[10] * in.m_m[11]) + (m_m[11] * in.m_m[15]);
	out.m_m[12] = (m_m[12] * in.m_m[0]) + (m_m[13] * in.m_m[4]) + (m_m[14] * in.m_m[8]) + (m_m[15] * in.m_m[12]);
	out.m_m[13] = (m_m[12] * in.m_m[1]) + (m_m[13] * in.m_m[5]) + (m_m[14] * in.m_m[9]) + (m_m[15] * in.m_m[13]);
	out.m_m[14] = (m_m[12] * in.m_m[2]) + (m_m[13] * in.m_m[6]) + (m_m[14] * in.m_m[10]) + (m_m[15] * in.m_m[14]);
	out.m_m[15] = (m_m[12] * in.m_m[3]) + (m_m[13] * in.m_m[7]) + (m_m[14] * in.m_m[11]) + (m_m[15] * in.m_m[15]);
}

Matrix4 Matrix4::Transpose() const
{
	Matrix4 out;
	out.m_m[0]	= m_m[0];
	out.m_m[1]	= m_m[4];
	out.m_m[2]	= m_m[8];
	out.m_m[3]	= m_m[12];
	out.m_m[4]	= m_m[1];
	out.m_m[5]	= m_m[5];
	out.m_m[6]	= m_m[9];
	out.m_m[7]	= m_m[13];
	out.m_m[8]	= m_m[2];
	out.m_m[9]	= m_m[6];
	out.m_m[10] = m_m[10];
	out.m_m[11] = m_m[14];
	out.m_m[12]	= m_m[3];
	out.m_m[13]	= m_m[7];
	out.m_m[14] = m_m[11];
	out.m_m[15] = m_m[15];
}

Matrix4& Matrix4::operator=(const Matrix3& in)
{
	m_m[0] = in.m_m[0];
	m_m[1] = in.m_m[1];
	m_m[2] = in.m_m[2];
	m_m[3] = 0.0f;
	m_m[4] = in.m_m[3];
	m_m[5] = in.m_m[4];
	m_m[6] = in.m_m[5];
	m_m[7] = 0.0f;
	m_m[8] = in.m_m[6];
	m_m[9] = in.m_m[7];
	m_m[10] = in.m_m[8];
	m_m[11] = 0.0f;
	m_m[12] = 0.0f;
	m_m[13] = 0.0f;
	m_m[14] = 0.0f;
	m_m[12] = 1.0f;
	return *this;
}

Matrix4& Matrix4::operator=(const Matrix4& in)
{
	m_m[0] = in.m_m[0];
	m_m[1] = in.m_m[1];
	m_m[2] = in.m_m[2];
	m_m[3] = in.m_m[3];
	m_m[4] = in.m_m[4];
	m_m[5] = in.m_m[5];
	m_m[6] = in.m_m[6];
	m_m[7] = in.m_m[7];
	m_m[8] = in.m_m[8];
	m_m[9] = in.m_m[9];
	m_m[10] = in.m_m[10];
	m_m[11] = in.m_m[11];
	m_m[12] = in.m_m[12];
	m_m[13] = in.m_m[13];
	m_m[14] = in.m_m[14];
	m_m[15] = in.m_m[15];
	return *this;
}

Vector4	Matrix4::GetRow(const Rows row) const
{
	const float* pFirst = m_m + (4 * row);
	return Vector4(pFirst[0], pFirst[1], pFirst[2], pFirst[3]);
}

} /* namespace Framework */
