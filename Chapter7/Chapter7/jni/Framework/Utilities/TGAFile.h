#ifndef TGAFILE_H_
#define TGAFILE_H_

#include "File.h"

namespace Framework
{
	class TGAFile
	{
	public:
		struct TGAHeader
		{
			unsigned char	m_idSize;
		    unsigned char	m_colorMapType;
		    unsigned char	m_imageType;

		    unsigned short	m_paletteStart;
		    unsigned short	m_paletteLength;
		    unsigned char	m_paletteBits;

		    unsigned short	m_xOrigin;
		    unsigned short	m_yOrigin;
		    unsigned short	m_width;
		    unsigned short	m_height;
		    unsigned char	m_bpp;
		    unsigned char	m_descriptor;
		} __attribute__ ((packed));

		TGAFile(void* pData);
		virtual ~TGAFile();

		unsigned short	GetWidth() const;
		unsigned short	GetHeight() const;
		void*			GetImageData() const;

	private:
		TGAHeader*	m_pHeader;
		void*		m_pImageData;
	};

	inline unsigned short TGAFile::GetWidth() const
	{
		unsigned short width = m_pHeader
				?	m_pHeader->m_width
				:	0;
		return width;
	}

	inline unsigned short TGAFile::GetHeight() const
	{
		unsigned short height = m_pHeader
				?	m_pHeader->m_height
				:	0;
		return height;
	}

	inline void* TGAFile::GetImageData() const
	{
		return m_pImageData;
	}
}

#endif // TGAFILE_H_
