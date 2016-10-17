#include "TransformShader.h"
#include "../Geometry/Geometry.h"
#include "../Renderer.h"

namespace Framework
{
	TransformShader::TransformShader()
	{
		m_vertexShaderCode =
				"uniform mat4 u_mModel;               		\n"
				"attribute vec4 a_vPosition;        		\n"
				"void main(){                       		\n"
				"    gl_Position = u_mModel * a_vPosition;	\n"
				"}                                  		\n";

		m_fragmentShaderCode =
				"precision highp float;    					\n"
				"uniform vec4 a_vColor;						\n"
				"void main(){                				\n"
				"    gl_FragColor = a_vColor;				\n"
				"}                         					\n";
	}

	TransformShader::~TransformShader()
	{

	}

	void TransformShader::Link()
	{
		Shader::Link();

		m_transformUniformHandle	= glGetUniformLocation(m_programId, "u_mModel");
		m_positionAttributeHandle	= glGetAttribLocation(m_programId, "a_vPosition");
		m_colorAttributeHandle		= glGetUniformLocation(m_programId, "a_vColor");
	}

	void TransformShader::Setup(Renderable& renderable)
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

			const Vector4& color = renderable.GetMaterial()->GetAmbientColor();
			glUniform4f(m_colorAttributeHandle, color.m_x, color.m_y, color.m_z, color.m_w);
		}
	}
}
