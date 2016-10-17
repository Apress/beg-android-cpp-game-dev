#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "Vector4.h"

namespace Framework
{

class Vector3
{
public:
	float m_x;
	float m_y;
	float m_z;

	Vector3();
	Vector3(const float x, const float y, const float z);
	virtual ~Vector3();

	void		Set(const Vector3& in);
	void		Multiply(const float scalar);
	void		Divide(const float scalar);
	void		Add(const Vector3& in);
	void		Subtract(const Vector3& in);
	void		Negate();
	float		Length() const;
	float		LengthSquared() const;
	void		Normalize();
	void		GetNormal(Vector3& normal);

	Vector3& operator=(const Vector3& in);
	Vector3& operator=(const Vector4& in);

	float	Dot(const Vector3& in) const;
	Vector3	Cross(const Vector3& in) const;
};

} /* namespace Framework */

#endif /* VECTOR3_H_ */
