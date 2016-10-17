#ifndef MATRIX3_H_
#define MATRIX3_H_

namespace Framework
{

class Vector3;
class Matrix4;

class Matrix3
{
public:
	float m_m[9];

	Matrix3();
	virtual ~Matrix3();

	void Identify();
	Vector3 Transform(const Vector3& in) const;
	Vector3 TransformTranspose(const Vector3& in) const;
	void RotateAroundX(const float radians);
	void RotateAroundY(const float radians);
	void RotateAroundZ(const float radians);

	Matrix3& operator=(const Matrix3& in);
	Matrix3& operator=(const Matrix4& in);

	Matrix3 Multiply(const Matrix3& input) const;
};

} /* namespace Framework */
#endif /* MATRIX3_H_ */
