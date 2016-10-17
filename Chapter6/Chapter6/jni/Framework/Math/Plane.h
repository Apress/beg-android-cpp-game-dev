#ifndef PLANE_H_
#define PLANE_H_

#include "Vector4.h"
#include "Vector3.h"

namespace Framework
{
	class Plane
	{
	private:
		Vector3		m_normal;
		float		m_d;

	public:
		Plane();
		Plane(const Vector3& point, const Vector3& normal);
		~Plane();

		void BuildPlane(const Vector3& point, const Vector3& normal);

		bool IsInFront(const Vector4& point) const;
		bool IsInFront(const Vector3& point) const;
	};
}

#endif // PLANE_H_
