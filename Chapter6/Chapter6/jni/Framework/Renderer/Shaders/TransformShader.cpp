#include "TransformShader.h"
#include "../Geometry/Geometry.h"

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

		float halfAngleRadians = 0.5f * 45.0f * (3.1415926536f / 180.0f);
		float m_top		= 1.0f * (float)tan(halfAngleRadians);
		float m_bottom	= -m_top;
		float m_right	= (1280.0f / 720.0f) * m_top;
		float m_left	= -m_right;
		float m_near	= 1.0f;
		float m_far		= 100.0f;

		m_projection.m_m[0] 	= (2.0f * m_near) / (m_right - m_left);
		m_projection.m_m[1] 	= 0.0f;
		m_projection.m_m[2] 	= 0.0f;
		m_projection.m_m[3] 	= 0.0f;
		m_projection.m_m[4] 	= 0.0f;
		m_projection.m_m[5] 	= (2.0f * m_near) / (m_top - m_bottom);
		m_projection.m_m[6] 	= 0.0f;
		m_projection.m_m[7] 	= 0.0f;
		m_projection.m_m[8] 	= -((m_right + m_left) / (m_right - m_left));
		m_projection.m_m[9]		= -((m_top + m_bottom) / (m_top - m_bottom));
		m_projection.m_m[10]	= (m_far + m_near) / (m_far - m_near);
		m_projection.m_m[11]	= 1.0f;
		m_projection.m_m[12]	= 0.0f;
		m_projection.m_m[13]	= 0.0f;
		m_projection.m_m[14]	= -(2.0f * m_near * m_far) / (m_far - m_near);
		m_projection.m_m[15]	= 0.0f;
	}

	void TransformShader::Setup(Renderable& renderable)
	{
		Geometry* pGeometry = renderable.GetGeometry();
		if (pGeometry)
		{
			Shader::Setup(renderable);

			Matrix4 mMVP;
			renderable.GetTransform().GetMatrix().Multiply(m_projection, mMVP);
			glUniformMatrix4fv(m_transformUniformHandle, 1, false, mMVP.m_m);

			glVertexAttribPointer(
					m_positionAttributeHandle,
					pGeometry->GetNumVertexPositionElements(),
					GL_FLOAT,
					GL_FALSE,
					pGeometry->GetVertexStride(),
					pGeometry->GetVertexBuffer());
			glEnableVertexAttribArray(m_positionAttributeHandle);

			Vector4& color = renderable.GetColor();
			glUniform4f(m_colorAttributeHandle, color.m_x, color.m_y, color.m_z, color.m_w);
		}
	}
}
