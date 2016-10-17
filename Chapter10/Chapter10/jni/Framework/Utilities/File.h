#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <android/asset_manager.h>

namespace Framework
{
	class File
	{
	private:
		std::string				m_filename;
		static AAssetManager*	m_pAssetManager;
		AAsset*					m_pAsset;

	public:
		explicit File(std::string name);
		virtual ~File();

		bool			Open();
		void			Read(void* pBuffer, const unsigned int bytesToRead, size_t& bytesRead);
		void			Close();

		unsigned int	Length() const;

		static const size_t READ_FAILED = 0xFFFFFFFF;

		static void 	SetAssetManager(AAssetManager* pAssetManager)	{ m_pAssetManager = pAssetManager; }
	};
}

#endif // FILE_H_
