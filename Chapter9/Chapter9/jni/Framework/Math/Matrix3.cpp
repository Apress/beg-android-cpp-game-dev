/*
 * Matrix3.cpp
 *
 *  Created on: 04/06/2012
 *      Author: bruce
 */

#include "Matrix3.h"
#include "Vector3.h"
#include "Matrix4.h"
#include <cmath>

namespace Framework
{

Matrix3::Matrix3()
{
}

Matrix3::~Matrix3()
{
}

void Matrix3::Identify()
{
	m_m[0] = 1.0f;
	m_m[1] = 0.0f;
	m_m[2] = 0.0f;
	m_m[3] = 0.0f;
	m_m[4] = 1.0f;
	m_m[5] = 0.0f;
	m_m[6] = 0.0f;
	m_m[7] = 0.0f;
	m_m[8] = 1.0f;
}

Vector3 Matrix3::Transform(const Vector3& in) const
{
	return Vector3(	(m_m[0] * in.m_x) + (m_m[1] * in.m_y) + (m_m[2] * in.m_y),
				(m_m[3] * in.m_x) + (m_m[4] * in.m_y) + (m_m[5] * in.m_y),
				(m_m[6] * in.m_x) + (m_m[7] * in.m_y) + (m_m[8] * in.m_y));
}

Vector3 Matrix3::TransformTranspose(const Vector3& in) const
{
	return Vector3((m_m[0] * in.m_x) + (m_m[3] * in.m_y) + (m_m[6] * in.m_y),
			(m_m[1] * in.m_x) + (m_m[4] * in.m_y) + (m_m[7] * in.m_y),
			(m_m[2] * in.m_x) + (m_m[5] * in.m_y) + (m_m[8] * in.m_y));
}

void Matrix3::RotateAroundX(const float radians)
{
	m_m[0] = 1.0f; m_m[1] = 0.0f; m_m[2] = 0.0f;
	m_m[3] = 0.0f; m_m[4] = cos(radians); m_m[5] = sin(radians);
	m_m[6] = 0.0f; m_m[7] = -sin(radians); m_m[8] = cos(radians);
}

void Matrix3::RotateAroundY(const float radians)
{
	m_m[0] = cos(radians); m_m[1] = 0.0f; m_m[2] = -sin(radians);
	m_m[3] = 0.0f; m_m[4] = 1.0f; m_m[5] = 0.0f;
	m_m[6] = sin(radians); m_m[7] = 0.0f; m_m[8] = cos(radians);
}

void Matrix3::RotateAroundZ(const float radians)
{
	m_m[0] = cos(radians); m_m[1] = sin(radians); m_m[2] = 0.0f;
	m_m[3] = -sin(radians); m_m[4] = cos(radians); m_m[5] = 0.0f;
	m_m[6] = 0.0f; m_m[7] = 0.0f; m_m[8] = 1.0f;
}

Matrix3& Matrix3::operator=(const Matrix3& in)
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
	return *this;
}

Matrix3& Matrix3::operator=(const Matrix4& in)
{
	m_m[0] = in.m_m[0];
	m_m[1] = in.m_m[1];
	m_m[2] = in.m_m[2];
	m_m[3] = in.m_m[4];
	m_m[4] = in.m_m[5];
	m_m[5] = in.m_m[6];
	m_m[6] = in.m_m[8];
	m_m[7] = in.m_m[9];
	m_m[8] = in.m_m[10];
	return *this;
}

Matrix3 Matrix3::Multiply(const Matrix3& input) const
{
	Matrix3 result;
	result.m_m[0] = (m_m[0] * input.m_m[0]) + (m_m[1] * input.m_m[3]) + (m_m[2] * input.m_m[6]);
	result.m_m[1] = (m_m[0] * input.m_m[1]) + (m_m[1] * input.m_m[4]) + (m_m[2] * input.m_m[7]);
	result.m_m[2] = (m_m[0] * input.m_m[2]) + (m_m[1] * input.m_m[5]) + (m_m[2] * input.m_m[8]);
	result.m_m[3] = (m_m[3] * input.m_m[0]) + (m_m[4] * input.m_m[3]) + (m_m[5] * input.m_m[6]);
	result.m_m[4] = (m_m[3] * input.m_m[1]) + (m_m[4] * input.m_m[4]) + (m_m[5] * input.m_m[7]);
	result.m_m[5] = (m_m[3] * input.m_m[2]) + (m_m[4] * input.m_m[5]) + (m_m[5] * input.m_m[8]);
	result.m_m[6] = (m_m[6] * input.m_m[0]) + (m_m[7] * input.m_m[3]) + (m_m[8] * input.m_m[6]);
	result.m_m[7] = (m_m[6] * input.m_m[1]) + (m_m[7] * input.m_m[4]) + (m_m[8] * input.m_m[7]);
	result.m_m[8] = (m_m[6] * input.m_m[2]) + (m_m[7] * input.m_m[5]) + (m_m[8] * input.m_m[8]);
	return result;
}

} /* namespace Framework */
