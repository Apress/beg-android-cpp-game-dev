#include "TransformAmbientShader.h"
#include "../Geometry/Geometry.h"
#include "../Renderer.h"

namespace Framework
{
	TransformAmbientShader::TransformAmbientShader()
	{
		m_vertexShaderCode =
				"uniform mat4 u_mModel;               		\n"
				"attribute vec4 a_vPosition;        		\n"
				"void main(){                       		\n"
				"    gl_Position = u_mModel * a_vPosition;	\n"
				"}                                  		\n";

		m_fragmentShaderCode =
				"precision highp float;    								\n"
				"														\n"
				"uniform vec4 u_vColor;									\n"
				"uniform vec4 u_vAmbientLight;							\n"
				"														\n"
				"void main(){                							\n"
				"    gl_FragColor = u_vAmbientLight * u_vColor;			\n"
				"}                         								\n";
	}

	TransformAmbientShader::~TransformAmbientShader()
	{

	}

	void TransformAmbientShader::Link()
	{
		Shader::Link();

		m_transformUniformHandle	= glGetUniformLocation(m_programId, "u_mModel");
		m_positionAttributeHandle	= glGetAttribLocation(m_programId, "a_vPosition");
		m_normalAttributeHandle		= glGetAttribLocation(m_programId, "a_vNormal");
		m_colorUniformHandle		= glGetUniformLocation(m_programId, "u_vColor");
		m_ambientLightUniformHandle	= glGetUniformLocation(m_programId, "u_vAmbientLight");
	}

	void TransformAmbientShader::Setup(Renderable& renderable)
	{
		Geometry* pGeometry = renderable.GetGeometry();
		if (pGeometry)
		{
			Shader::Setup(renderable);

			Renderer& renderer = Renderer::GetSingleton();
			const Matrix4& viewMatrix = renderer.GetViewMatrix();
			const Matrix4& projectionMatrix = renderer.GetProjectionMatrix();

			Matrix4 modelViewMatrix;
			renderable.GetTransform().GetMatrix().Multiply(viewMatrix, modelViewMatrix);

			Matrix4 modelViewProjectionMatrix;
			modelViewMatrix.Multiply(projectionMatrix, modelViewProjectionMatrix);

			glUniformMatrix4fv(m_transformUniformHandle, 1, false, modelViewProjectionMatrix.m_m);

			glVertexAttribPointer(
					m_positionAttributeHandle,
					pGeometry->GetNumVertexPositionElements(),
					GL_FLOAT,
					GL_FALSE,
					pGeometry->GetVertexStride(),
					pGeometry->GetVertexBuffer());
			glEnableVertexAttribArray(m_positionAttributeHandle);

			glVertexAttribPointer(
					m_normalAttributeHandle,
					pGeometry->GetNumNormalPositionElements(),
					GL_FLOAT,
					GL_FALSE,
					pGeometry->GetVertexStride(),
					static_cast<float*>(pGeometry->GetVertexBuffer()) + pGeometry->GetNumVertexPositionElements());
			glEnableVertexAttribArray(m_normalAttributeHandle);

			const Vector4& color = renderable.GetMaterial()->GetAmbientColor();
			glUniform4f(m_colorUniformHandle, color.m_x, color.m_y, color.m_z, color.m_w);

			const Vector4& ambientLightColor = renderer.GetAmbientLightColor();
			glUniform4f(m_ambientLightUniformHandle,
					ambientLightColor.m_x,
					ambientLightColor.m_y,
					ambientLightColor.m_z,
					ambientLightColor.m_w);
		}
	}
}
