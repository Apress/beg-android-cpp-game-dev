#include "Vector3.h"
#include <cmath>

namespace Framework
{

Vector3::Vector3()
	: 	m_x(0.0f)
	,	m_y(0.0f)
	,	m_z(0.0f)
{
}

Vector3::Vector3(const float x, const float y, const float z)
	: 	m_x(x)
	,	m_y(y)
	,	m_z(z)
{
}

Vector3::~Vector3()
{
}

void Vector3::Set(const Vector3& in) {
	m_x = in.m_x;
	m_y = in.m_y;
	m_z = in.m_z;
}

void Vector3::Multiply(const float scalar) {
	m_x *= scalar;
	m_y *= scalar;
	m_z *= scalar;
}

void Vector3::Divide(const float scalar) {
	float divisor = 1.0f / scalar;
	m_x *= divisor;
	m_y *= divisor;
	m_z *= divisor;
}

void Vector3::Add(const Vector3& in) {
	m_x += in.m_x;
	m_y += in.m_y;
	m_z += in.m_z;
}

void Vector3::Subtract(const Vector3& in) {
	m_x -= in.m_x;
	m_y -= in.m_y;
	m_z -= in.m_z;
}

void Vector3::Negate()
{
	m_x = -m_x;
	m_y = -m_y;
	m_z = -m_z;
}

Vector3& Vector3::operator=(const Vector3& in)
{
	m_x = in.m_x;
	m_y = in.m_y;
	m_z = in.m_z;
	return *this;
}

Vector3& Vector3::operator=(const Vector4& in)
{
	m_x = in.m_x;
	m_y = in.m_y;
	m_z = in.m_z;
	return *this;
}

float Vector3::Length() const
{
	return sqrt((m_x*m_x) + (m_y*m_y) + (m_z*m_z));
}

float Vector3::LengthSquared() const
{
	return (m_x*m_x) + (m_y*m_y) + (m_z*m_z);
}

void Vector3::Normalize()
{
	Divide(Length());
}

void Vector3::GetNormal(Vector3& normal)
{
	normal = *this;
	normal.Normalize();
}

float Vector3::Dot(const Vector3& in) const
{
	return (m_x * in.m_x) + (m_y * in.m_y) + (m_z * in.m_z);
}

Vector3 Vector3::Cross(const Vector3& in) const
{
	return Vector3(
			(m_y * in.m_z) - (m_z * in.m_y),
			(m_z * in.m_x) - (m_x * in.m_z),
			(m_x * in.m_y) - (m_y * in.m_x));
}

} /* namespace Framework */
