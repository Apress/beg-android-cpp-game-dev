#include "AudioManager.h"
#include <cstddef>
#include "../Utilities/Hash.h"
#include <SLES/OpenSLES_Android.h>

namespace Framework
{
	AudioManager::AudioManager(AAssetManager* pAssetManager)
		:	m_engineObject(NULL)
		,	m_engine(NULL)
		,	m_outputMixObject(NULL)
		,	m_pAssetManager(pAssetManager)
	{
		Initialize();
	}

	AudioManager::~AudioManager()
	{
		Destroy();
	}

	bool AudioManager::Initialize()
	{
		SLresult result;

		result = slCreateEngine( &m_engineObject, 0, NULL, 0, NULL, NULL );
		assert(result == SL_RESULT_SUCCESS);

		result = (*m_engineObject)->Realize(m_engineObject, SL_BOOLEAN_FALSE);
		assert(result == SL_RESULT_SUCCESS);

		result = (*m_engineObject)->GetInterface(m_engineObject, SL_IID_ENGINE, &m_engine);
		assert(result == SL_RESULT_SUCCESS);

		result = (*m_engine)->CreateOutputMix(m_engine, &m_outputMixObject, 0, NULL, NULL);
		assert(result == SL_RESULT_SUCCESS);

		result = (*m_outputMixObject)->Realize(m_outputMixObject, SL_BOOLEAN_FALSE);
		assert(result == SL_RESULT_SUCCESS);

		return result == SL_RESULT_SUCCESS;
	}

	void AudioManager::Destroy()
	{
		for (PlayInstanceMapIterator iter = m_playInstances.begin(); iter != m_playInstances.end(); ++iter)
		{
			PlayInstance* pInstance = iter->second;
			if (pInstance && pInstance->m_playerObject)
			{
				(*pInstance->m_playerObject)->Destroy(pInstance->m_playerObject);
				pInstance->m_playerObject	= NULL;
				pInstance->m_playerPlay		= NULL;
			}
		}
		m_playInstances.clear();

		if (m_outputMixObject != NULL)
		{
			(*m_outputMixObject)->Destroy(m_outputMixObject);
			m_outputMixObject = NULL;
		}

		if (m_engineObject != NULL)
		{
			(*m_engineObject)->Destroy(m_engineObject);
			m_engineObject = NULL;
			m_engine = NULL;
		}
	}

	void SLAPIENTRY play_callback( SLPlayItf player, void *context, SLuint32 event )
	{
	    if( event & SL_PLAYEVENT_HEADATEND )
	    {
	    	(*player)->SetPlayState(player, SL_PLAYSTATE_STOPPED);
	    }
	}

	AudioManager::AudioHandle AudioManager::CreateSFX(std::string& filename)
	{
		AudioHandle handle = INVALID_HANDLE;

		AAsset* asset = AAssetManager_open(m_pAssetManager, filename.c_str(), AASSET_MODE_UNKNOWN);

		if (asset != NULL)
		{
			handle = Hash(filename.c_str());

			PlayInstanceMapIterator iter = m_playInstances.find(handle);
			if (iter == m_playInstances.end())
			{
				PlayInstance* pNewInstance = new PlayInstance();

				if (pNewInstance)
				{
					std::pair<AudioHandle, PlayInstance*> newInstance(handle, pNewInstance);
					std::pair<PlayInstanceMapIterator, bool> addedIter = m_playInstances.insert(newInstance);
				}

				off_t start;
				off_t length;
				int fd = AAsset_openFileDescriptor(asset, &start, &length);
				assert(0 <= fd);
				AAsset_close(asset);

				// configure audio source
				SLDataLocator_AndroidFD loc_fd = {SL_DATALOCATOR_ANDROIDFD, fd, start, length};
				SLDataFormat_MIME format_mime = {SL_DATAFORMAT_MIME, NULL, SL_CONTAINERTYPE_UNSPECIFIED};
				SLDataSource audioSrc = {&loc_fd, &format_mime};

				// configure audio sink
				SLDataLocator_OutputMix loc_outmix = {SL_DATALOCATOR_OUTPUTMIX, m_outputMixObject};
				SLDataSink audioSnk = {&loc_outmix, NULL};

				// create audio player
				const unsigned int NUM_INTERFACES = 1;
				const SLInterfaceID ids[NUM_INTERFACES] = {SL_IID_PLAY};
				const SLboolean req[NUM_INTERFACES]		= {SL_BOOLEAN_TRUE};
				SLresult result = (*m_engine)->CreateAudioPlayer(m_engine, &pNewInstance->m_playerObject, &audioSrc, &audioSnk, NUM_INTERFACES, ids, req);
				assert(SL_RESULT_SUCCESS == result);

				// realize the player
				result = (*pNewInstance->m_playerObject)->Realize(pNewInstance->m_playerObject, SL_BOOLEAN_FALSE);
				assert(SL_RESULT_SUCCESS == result);

				// get the play interface
				result = (*pNewInstance->m_playerObject)->GetInterface(pNewInstance->m_playerObject, SL_IID_PLAY, &pNewInstance->m_playerPlay);
				assert(SL_RESULT_SUCCESS == result);
				(*pNewInstance->m_playerPlay)->RegisterCallback( pNewInstance->m_playerPlay, play_callback, NULL );
				(*pNewInstance->m_playerPlay)->SetCallbackEventsMask( pNewInstance->m_playerPlay, SL_PLAYEVENT_HEADATEND );
			}
		}

		return handle;
	}

	void AudioManager::PlaySFX(AudioHandle handle)
	{
		PlayInstanceMapIterator iter = m_playInstances.find(handle);
		if (iter != m_playInstances.end())
		{
			SLPlayItf pPlayInstance = iter->second->m_playerPlay;
			if (pPlayInstance != NULL)
			{
				// set the player's state
				(*pPlayInstance)->SetPlayState(pPlayInstance, SL_PLAYSTATE_STOPPED);
				(*pPlayInstance)->SetPlayState(pPlayInstance, SL_PLAYSTATE_PLAYING);
			}
		}
	}

	void AudioManager::DestroySFX(AudioHandle handle)
	{
		PlayInstanceMapIterator iter = m_playInstances.find(handle);
		if (iter != m_playInstances.end())
		{
			PlayInstance* pInstance = iter->second;
			if (pInstance && pInstance->m_playerObject)
			{
				(*pInstance->m_playerObject)->Destroy(pInstance->m_playerObject);
				pInstance->m_playerObject	= NULL;
				pInstance->m_playerPlay		= NULL;
			}

			m_playInstances.erase(iter);
		}
	}
}
