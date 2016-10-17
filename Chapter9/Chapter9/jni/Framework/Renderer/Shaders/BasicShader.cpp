#include "BasicShader.h"
#include <cassert>
#include "../Geometry/Geometry.h"

namespace Framework
{
	BasicShader::BasicShader()
	{
		m_vertexShaderCode =
				"attribute vec4 a_vPosition;        \n"
				"void main(){                       \n"
				"     gl_Position = a_vPosition;	\n"
				"}                                  \n";

		m_fragmentShaderCode =
				"precision mediump float;  						\n"
				"void main(){              						\n"
				"    gl_FragColor = vec4(0.2, 0.2, 0.2, 1.0);   \n"
				"}                         						\n";
	}

	BasicShader::~BasicShader()
	{

	}

	void BasicShader::Link()
	{
		Shader::Link();

		m_positionAttributeHandle = glGetAttribLocation(m_programId, "a_vPosition");
	}

	void BasicShader::Setup(Renderable& renderable)
	{
		Shader::Setup(renderable);

		Geometry* pGeometry = renderable.GetGeometry();
		assert(pGeometry);

		glVertexAttribPointer(
				m_positionAttributeHandle,
				pGeometry->GetNumVertexPositionElements(),
				GL_FLOAT,
				GL_FALSE,
				pGeometry->GetVertexStride(),
				pGeometry->GetVertexBuffer());
		glEnableVertexAttribArray(m_positionAttributeHandle);
	}
}
