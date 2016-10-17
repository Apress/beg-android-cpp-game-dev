#include "File.h"
#include <cassert>

namespace Framework
{
	AAssetManager* File::m_pAssetManager = NULL;

	File::File(std::string name)
		:	m_filename(name)
	{
		assert(m_pAssetManager != NULL);
		m_pAsset = NULL;
	}

	File::~File()
	{
		Close();
	}

	bool File::Open()
	{
		assert(m_pAssetManager != NULL);

		m_pAsset = AAssetManager_open(m_pAssetManager, m_filename.c_str(), AASSET_MODE_UNKNOWN);
		return !!m_pAsset;
	}

	void File::Read(void* pBuffer, const unsigned int bytesToRead, size_t& bytesRead)
	{
		bytesRead = AAsset_read(m_pAsset, pBuffer, bytesToRead);
	}

	void File::Close()
	{
		if (m_pAsset)
		{
			AAsset_close(m_pAsset);
			m_pAsset = NULL;
		}
	}

	unsigned int File::Length() const
	{
		return AAsset_getLength(m_pAsset);
	}
}
