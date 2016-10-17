#include "Transform.h"

namespace Framework
{

Transform::Transform()
{
	m_rotation.Identify();
	m_translation = Vector3(0.0f, 0.0f, 0.0f);
	m_scale = 1.0f;
	m_matrix.Identify();
}

Transform::~Transform()
{
}

void Transform::Clone(const Transform& transform)
{
	m_rotation = transform.m_rotation;
	m_translation = transform.m_translation;
	m_scale = transform.m_scale;

	UpdateMatrix();
}

void Transform::SetRotation(const Matrix3& rotation)
{
	m_rotation = rotation;
}

const Matrix3& Transform::GetRotation() const
{
	return m_rotation;
}

void Transform::SetTranslation(const Vector3& translation)
{
	m_translation = translation;
}

const Vector3& Transform::GetTranslation() const
{
	return m_translation;
}

void Transform::SetScale(const float scale)
{
	m_scale = scale;
}

const float Transform::GetScale() const
{
	return m_scale;
}

void Transform::ApplyForward(const Vector3& in, Vector3& out) const
{
	out.Set(in);

	out.Multiply(m_scale);
	m_rotation.Transform(out);
	out.Add(m_translation);
}

void Transform::ApplyInverse(const Vector3& in, Vector3& out) const
{
	out.Set(in);

	out.Subtract(m_translation);
	m_rotation.TransformTranspose(out);
	out.Divide(m_scale);
}

void Transform::UpdateMatrix()
{
	m_matrix.m_m[0] = m_rotation.m_m[0] * m_scale;
	m_matrix.m_m[1] = m_rotation.m_m[1];
	m_matrix.m_m[2] = m_rotation.m_m[2];
	m_matrix.m_m[3] = 0.0f;
	m_matrix.m_m[4] = m_rotation.m_m[3];
	m_matrix.m_m[5] = m_rotation.m_m[4] * m_scale;
	m_matrix.m_m[6] = m_rotation.m_m[5];
	m_matrix.m_m[7] = 0.0f;
	m_matrix.m_m[8] = m_rotation.m_m[6];
	m_matrix.m_m[9] = m_rotation.m_m[7];
	m_matrix.m_m[10] = m_rotation.m_m[8] * m_scale;
	m_matrix.m_m[11] = 0.0f;
	m_matrix.m_m[12] = m_translation.m_x;
	m_matrix.m_m[13] = m_translation.m_y;
	m_matrix.m_m[14] = m_translation.m_z;
	m_matrix.m_m[15] = 1.0f;
}

const Matrix4& Transform::GetMatrix() const
{
	return m_matrix;
}

void Transform::GetInverseMatrix(Matrix4& out) const
{
	float invScale = 1.0f / m_scale;
	out.m_m[0] = m_rotation.m_m[0] * invScale;
	out.m_m[1] = m_rotation.m_m[3];
	out.m_m[2] = m_rotation.m_m[6];
	out.m_m[3] = 0.0f;
	out.m_m[4] = m_rotation.m_m[1];
	out.m_m[5] = m_rotation.m_m[4] * invScale;
	out.m_m[6] = m_rotation.m_m[7];
	out.m_m[7] = 0.0f;
	out.m_m[8] = m_rotation.m_m[2];
	out.m_m[9] = m_rotation.m_m[5];
	out.m_m[10] = m_rotation.m_m[8] * invScale;
	out.m_m[11] = 0.0f;
	out.m_m[12] = -m_translation.m_x;
	out.m_m[13] = -m_translation.m_y;
	out.m_m[14] = -m_translation.m_z;
	out.m_m[15] = 1.0f;
}

void Transform::GetInverseTransposeMatrix(Matrix4& out) const
{
	float invScale = 1.0f / m_scale;
	out.m_m[0] = m_rotation.m_m[0] * invScale;
	out.m_m[1] = m_rotation.m_m[1];
	out.m_m[2] = m_rotation.m_m[2];
	out.m_m[3] = 0.0f;
	out.m_m[4] = m_rotation.m_m[3];
	out.m_m[5] = m_rotation.m_m[4] * invScale;
	out.m_m[6] = m_rotation.m_m[5];
	out.m_m[7] = 0.0f;
	out.m_m[8] = m_rotation.m_m[6];
	out.m_m[9] = m_rotation.m_m[7];
	out.m_m[10] = m_rotation.m_m[8] * invScale;
	out.m_m[11] = 0.0f;
	out.m_m[12] = -m_translation.m_x;
	out.m_m[13] = -m_translation.m_y;
	out.m_m[14] = -m_translation.m_z;
	out.m_m[15] = 1.0f;
}

void Transform::GetInverseTransposeMatrix(Matrix3& out) const
{
	float invScale = 1.0f / m_scale;
	out.m_m[0] = m_rotation.m_m[0] * invScale;
	out.m_m[1] = m_rotation.m_m[1];
	out.m_m[2] = m_rotation.m_m[2];
	out.m_m[3] = m_rotation.m_m[3];
	out.m_m[4] = m_rotation.m_m[4] * invScale;
	out.m_m[5] = m_rotation.m_m[5];
	out.m_m[6] = m_rotation.m_m[6];
	out.m_m[7] = m_rotation.m_m[7];
	out.m_m[8] = m_rotation.m_m[8] * invScale;
}

Transform Transform::Multiply(const Transform& input) const
{
	Transform result;

	result.m_rotation = m_rotation.Multiply(input.GetRotation());

	result.m_translation.Set(result.m_rotation.Transform(input.GetTranslation()));
	result.m_translation.Multiply(m_scale);
	result.m_translation.Add(m_translation);

	result.m_scale = m_scale * input.GetScale();

	return result;
}

} /* namespace Framework */
