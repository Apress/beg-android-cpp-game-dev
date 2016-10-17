#include "TransformAmbientDiffuseShader.h"
#include "../Geometry/Geometry.h"
#include "../Renderer.h"

namespace Framework
{
	TransformAmbientDiffuseShader::TransformAmbientDiffuseShader()
	{
		m_vertexShaderCode =
				"uniform mat4 u_mModelViewProj;             			\n"
				"uniform mat3 u_mModelIT;               				\n"
				"attribute vec4 a_vPosition;        					\n"
				"attribute vec3 a_vNormal; 		       					\n"
				"varying   vec3 v_vNormal; 		       					\n"
				"void main(){                       					\n"
				"    gl_Position = u_mModelViewProj * a_vPosition;		\n"
				"    v_vNormal = normalize(u_mModelIT * a_vNormal);		\n"
				"}                                  					\n";

		m_fragmentShaderCode =
				"precision highp float;  										\n"
				"varying vec3 v_vNormal;										\n"
				"																\n"
				"uniform vec4 u_vAmbientColor;									\n"
				"uniform vec4 u_vDiffuseColor;									\n"
				"uniform vec4 u_vAmbientLight;									\n"
				"uniform vec4 u_vDiffuseLight;									\n"
				"uniform vec3 u_vLightDirection;								\n"
				"																\n"
				"const float c_zero = 0.0;										\n"
				"const float c_one  = 1.0;										\n"
				"																\n"
				"void main(){                									\n"
				"    gl_FragColor = vec4(c_zero, c_zero, c_zero, c_zero); 		\n"
				"																\n"
				"    float ndotl = dot(u_vLightDirection, v_vNormal);			\n"
				" 	 ndotl = max(ndotl, c_zero);								\n"
				"	 gl_FragColor += ndotl * u_vDiffuseLight * u_vDiffuseColor;	\n"
				"	 															\n"
				"    gl_FragColor += u_vAmbientLight * u_vAmbientColor;			\n"
				"                         										\n"
				"    gl_FragColor.a = c_one;									\n"
				"}                         										\n";
	}

	TransformAmbientDiffuseShader::~TransformAmbientDiffuseShader()
	{

	}

	void TransformAmbientDiffuseShader::Link()
	{
		Shader::Link();

		m_modelViewProjUniformHandle	= glGetUniformLocation(m_programId, "u_mModelViewProj");
		m_modelITMatrixUniformHandle	= glGetUniformLocation(m_programId, "u_mModelIT");
		m_positionAttributeHandle		= glGetAttribLocation(m_programId, "a_vPosition");
		m_normalAttributeHandle			= glGetAttribLocation(m_programId, "a_vNormal");
		m_ambientColorUniformHandle		= glGetUniformLocation(m_programId, "u_vAmbientColor");
		m_diffuseColorUniformHandle		= glGetUniformLocation(m_programId, "u_vDiffuseColor");
		m_ambientLightUniformHandle		= glGetUniformLocation(m_programId, "u_vAmbientLight");
		m_diffuseLightUniformHandle		= glGetUniformLocation(m_programId, "u_vDiffuseLight");
		m_lightDirectionUniformHandle	= glGetUniformLocation(m_programId, "u_vLightDirection");
	}

	void TransformAmbientDiffuseShader::Setup(Renderable& renderable)
	{
		Geometry* pGeometry = renderable.GetGeometry();
		if (pGeometry)
		{
			Shader::Setup(renderable);

			Renderer& renderer = Renderer::GetSingleton();
			const Matrix4& viewMatrix = renderer.GetViewMatrix();
			const Matrix4& projectionMatrix = renderer.GetProjectionMatrix();

			const Matrix4& modelMatrix = renderable.GetTransform().GetMatrix();

			Matrix4 modelViewMatrix;
			modelMatrix.Multiply(viewMatrix, modelViewMatrix);

			Matrix4 modelViewProjectionMatrix;
			modelViewMatrix.Multiply(projectionMatrix, modelViewProjectionMatrix);

			glUniformMatrix4fv(m_modelViewProjUniformHandle, 1, false, modelViewProjectionMatrix.m_m);

			Matrix3 modelIT;
			renderable.GetTransform().GetInverseTransposeMatrix(modelIT);
			glUniformMatrix3fv(m_modelITMatrixUniformHandle, 1, false, modelIT.m_m);

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

			const Vector4& ambientColor = renderable.GetMaterial()->GetAmbientColor();
			glUniform4f(m_ambientColorUniformHandle, ambientColor.m_x, ambientColor.m_y, ambientColor.m_z, ambientColor.m_w);

			const Vector4& diffuseColor = renderable.GetMaterial()->GetDiffuseColor();
			glUniform4f(m_diffuseColorUniformHandle, diffuseColor.m_x, diffuseColor.m_y, diffuseColor.m_z, diffuseColor.m_w);

			const Vector4& ambientLightColor = renderer.GetAmbientLightColor();
			glUniform4f(m_ambientLightUniformHandle,
					ambientLightColor.m_x,
					ambientLightColor.m_y,
					ambientLightColor.m_z,
					ambientLightColor.m_w);

			const Vector4& diffuseLightColor = renderer.GetDiffuseLightColor();
			glUniform4f(m_diffuseLightUniformHandle,
					diffuseLightColor.m_x,
					diffuseLightColor.m_y,
					diffuseLightColor.m_z,
					diffuseLightColor.m_w);

			const Vector3& lightDirection = renderer.GetLightDirection();
			glUniform3f(m_lightDirectionUniformHandle,
					lightDirection.m_x,
					lightDirection.m_y,
					lightDirection.m_z);
		}
	}
}
