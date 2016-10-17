#include "Plane.h"

namespace Framework
{
	Plane::Plane()
		:	m_d(0.0f)
	{
	}

	Plane::Plane(const Vector3& point, const Vector3& normal)
	{
		BuildPlane(point, normal);
	}

	Plane::~Plane()
	{

	}

	void Plane::BuildPlane(const Vector3& point, const Vector3& normal)
	{
		m_normal	= normal;
		m_normal.Normalize();
		m_d			= m_normal.Dot(point);
	}

	bool Plane::IsInFront(const Vector4& point) const
	{
		return IsInFront(Vector3(point.m_x, point.m_y, point.m_z));
	}

	bool Plane::IsInFront(const Vector3& point) const
	{
		return m_normal.Dot(point) >= m_d;
	}
}
