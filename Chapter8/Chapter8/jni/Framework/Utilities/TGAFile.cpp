#include "TGAFile.h"

namespace Framework
{
	TGAFile::TGAFile(void* pData)
	{
		size_t size = sizeof(unsigned short);
		m_pHeader		= static_cast<TGAHeader*>(pData);
		m_pImageData	= static_cast<void*>(static_cast<char*>(pData) + sizeof(TGAHeader));
	}

	TGAFile::~TGAFile()
	{

	}
}
