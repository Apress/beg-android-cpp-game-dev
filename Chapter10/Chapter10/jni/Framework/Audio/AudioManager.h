#ifndef AUDIOMANAGER_H_
#define AUDIOMANAGER_H_

#include "../Utilities/Singleton.h"
#include <SLES/OpenSLES.h>
#include <android/asset_manager.h>
#include <unordered_map>
#include <string>

namespace Framework
{
	class AudioManager
		:	public Singleton<AudioManager>
	{
	public:
		typedef unsigned int		AudioHandle;
		static const AudioHandle	INVALID_HANDLE	= 0xFFFFFFFF;

	private:
		SLObjectItf					m_engineObject;
		SLEngineItf					m_engine;
		SLObjectItf 				m_outputMixObject;

		AAssetManager*				m_pAssetManager;

		struct PlayInstance
		{
			SLObjectItf 		m_playerObject;
			SLPlayItf			m_playerPlay;
		};
		typedef std::tr1::unordered_map<AudioHandle, PlayInstance*>		PlayInstanceMap;
		typedef PlayInstanceMap::iterator								PlayInstanceMapIterator;

		PlayInstanceMap				m_playInstances;

	public:
		explicit AudioManager(AAssetManager* pAssetManager);
		virtual ~AudioManager();

		bool	Initialize();
		void	Destroy();

		AudioHandle	CreateSFX(std::string& filename);
		void		PlaySFX(AudioHandle handle);
		void		DestroySFX(AudioHandle handle);
	};
}

#endif // AUDIOMANAGER_H_
