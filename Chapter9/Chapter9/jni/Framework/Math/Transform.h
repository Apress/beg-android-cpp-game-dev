#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#include "Matrix3.h"
#include "Matrix4.h"
#include "Vector3.h"

namespace Framework
{

class Transform
{
private:
	Matrix3		m_rotation;
	Vector3		m_translation;
	float		m_scale;
	Matrix4		m_matrix;

public:
	Transform();
	virtual ~Transform();

	void 				Clone(const Transform& transform);

	void 				SetRotation(const Matrix3& rotation);
	const Matrix3& 		GetRotation() const;

	void 				SetTranslation(const Vector3& translation);
	const Vector3& 		GetTranslation() const;

	void 				SetScale(const float scale);
	const float 		GetScale() const;

	void 				ApplyForward(const Vector3& in, Vector3& out) const;
	void 				ApplyInverse(const Vector3& in, Vector3& out) const;

	void 				UpdateMatrix();

	const Matrix4&		GetMatrix() const;
	void 				GetInverseMatrix(Matrix4& out) const;
	void 				GetInverseTransposeMatrix(Matrix4& out) const;
	void 				GetInverseTransposeMatrix(Matrix3& out) const;

	Transform			Multiply(const Transform& input) const;
};

} /* namespace Framework */
#endif /* TRANSFORM_H_ */
