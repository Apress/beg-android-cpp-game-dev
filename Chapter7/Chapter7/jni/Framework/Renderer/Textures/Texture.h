#ifndef TEXTURE_H_
#define TEXTURE_H_

#include <GLES2/gl2.h>

namespace Framework
{
	class Texture
	{
	public:
		struct Header
		{
			unsigned int	m_width;
			unsigned int	m_height;
			unsigned int	m_bytesPerPixel;
			unsigned int	m_dataSize;

			Header()
				:	m_width(0)
				,	m_height(0)
				,	m_bytesPerPixel(0)
				,	m_dataSize(0)
			{

			}

			Header(const Header& header)
			{
				m_width			= header.m_width;
				m_height		= header.m_height;
				m_bytesPerPixel = header.m_bytesPerPixel;
				m_dataSize		= header.m_dataSize;
			}
		};

	private:
		GLuint		m_id;
		Header		m_header;
		void*		m_pImageData;

	public:
		Texture();
		~Texture();

		void SetData(Header& header, void* pImageData);

		GLuint GetId() const { return m_id; }

		void Init();
	};
}

#endif // TEXTURE_H_
