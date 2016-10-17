#include "Texture.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

namespace Framework
{
	Texture::Texture()
		:	m_id(GL_INVALID_VALUE)
	{

	}

	Texture::~Texture()
	{

	}

	void Texture::SetData(Texture::Header& header, void* pImageData)
	{
		m_header		= header;
		m_pImageData	= pImageData;
	}

	void Texture::Init()
	{
		GLint	packBits		= 4;
		GLint	internalFormat	= GL_RGBA;
		GLenum	format			= GL_RGBA;
		switch (m_header.m_bytesPerPixel)
		{
		case 1:
			{
				packBits		= 1;
				internalFormat	= GL_ALPHA;
				format			= GL_ALPHA;
			}
			break;
		};

		glGenTextures(1, &m_id);

		glBindTexture(GL_TEXTURE_2D, m_id);

		glPixelStorei(GL_UNPACK_ALIGNMENT, packBits);

		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_header.m_width, m_header.m_height, 0, format, GL_UNSIGNED_BYTE, m_pImageData);
	}
}
