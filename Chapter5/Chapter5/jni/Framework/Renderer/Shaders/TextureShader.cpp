#include "TextureShader.h"
#include <cassert>
#include "../Geometry/Geometry.h"
#include "../Textures/Texture.h"

namespace Framework
{
	TextureShader::TextureShader()
		:	m_pTexture(NULL)
	{
		m_vertexShaderCode =
				"attribute vec4 a_vPosition;        \n"
				"attribute vec2 a_texCoord;         \n"
				"varying   vec2 v_texCoord;         \n"
				"void main(){                       \n"
				"    gl_Position = a_vPosition;		\n"
				"    v_texCoord = a_texCoord;		\n"
				"}                                  \n";

		m_fragmentShaderCode =
				"precision highp float;    							\n"
				"varying vec2 v_texCoord;								\n"
				"uniform sampler2D s_texture;							\n"
				"void main(){                							\n"
				"    gl_FragColor = texture2D(s_texture, v_texCoord);   \n"
				"}                         								\n";
	}

	TextureShader::~TextureShader()
	{

	}

	void TextureShader::Link()
	{
		Shader::Link();

		m_positionAttributeHandle	= glGetAttribLocation(m_programId, "a_vPosition");
		m_texCoordAttributeHandle	= glGetAttribLocation(m_programId, "a_texCoord");
		m_samplerHandle				= glGetUniformLocation(m_programId, "s_texture");
	}

	void TextureShader::Setup(Renderable& renderable)
	{
		assert(m_pTexture);
		Geometry* pGeometry = renderable.GetGeometry();
		if (pGeometry && m_pTexture)
		{
			Shader::Setup(renderable);

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_pTexture->GetId());
			glUniform1i(m_samplerHandle, 0);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glVertexAttribPointer(
					m_positionAttributeHandle,
					pGeometry->GetNumVertexPositionElements(),
					GL_FLOAT,
					GL_FALSE,
					pGeometry->GetVertexStride(),
					pGeometry->GetVertexBuffer());
			glEnableVertexAttribArray(m_positionAttributeHandle);

			glVertexAttribPointer(
					m_texCoordAttributeHandle,
					pGeometry->GetNumTexCoordElements(),
					GL_FLOAT,
					GL_FALSE,
					pGeometry->GetVertexStride(),
					&static_cast<GLfloat*>(pGeometry->GetVertexBuffer())[pGeometry->GetNumVertexPositionElements()]);
			glEnableVertexAttribArray(m_texCoordAttributeHandle);
		}
	}
}
