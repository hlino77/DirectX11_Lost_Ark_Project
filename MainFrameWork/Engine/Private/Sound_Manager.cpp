#include "Sound_Manager.h"
#include "PipeLine.h"
#include "AsUtils.h"
#include <filesystem>

namespace fs = std::filesystem;

IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{

}

HRESULT CSound_Manager::Ready_Sound()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	FMOD_System_Init(m_pSystem, 200, FMOD_INIT_NORMAL, NULL);

	if (nullptr == m_pSystem)
		return E_FAIL;

	wstring strLoadpath = (L"../Bin/Resources/Sounds/");
	if (FAILED(LoadDirectory(strLoadpath)))
		return E_FAIL;

	return S_OK;
}

HRESULT CSound_Manager::Add_ChannelGroup(const string& strChannelGroupName, _float fVolume)
{
	if (m_ChannelGroups.find(CAsUtils::ToWString(strChannelGroupName)) != m_ChannelGroups.end())
		return E_FAIL;

	FMOD_CHANNELGROUP* pChannelGroup = nullptr;
	FMOD_System_CreateChannelGroup(m_pSystem, strChannelGroupName.c_str(), &pChannelGroup);
	FMOD_ChannelGroup_SetVolume(pChannelGroup, fVolume);

	m_ChannelGroups.emplace(CAsUtils::ToWString(strChannelGroupName), pChannelGroup);

	FMOD_System_Update(m_pSystem);
	return S_OK;
}


HRESULT CSound_Manager::PlaySoundFile(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume, FMOD_CHANNEL** pChannel)
{
	auto& Sounditer = m_Sounds.find(strSoundKey);

	if (Sounditer == m_Sounds.end())
		return E_FAIL;

	auto& Groupiter = m_ChannelGroups.find(szChannelGroup);

	if (Groupiter == m_ChannelGroups.end())
		return E_FAIL;

	FMOD_CHANNEL* pNewChannel = nullptr;

	FMOD_System_PlaySound(m_pSystem, Sounditer->second, Groupiter->second, FALSE, &pNewChannel);
	FMOD_Channel_SetVolume(pNewChannel, fVolume);
	FMOD_System_Update(m_pSystem);

	if (pChannel != nullptr)
	{
		*pChannel = pNewChannel;
	}

	return S_OK;
}

HRESULT CSound_Manager::PlayBGM(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume, FMOD_CHANNEL** pChannel)
{
	auto& Sounditer = m_Sounds.find(strSoundKey);

	if (Sounditer == m_Sounds.end())
		return E_FAIL;

	auto& Groupiter = m_ChannelGroups.find(szChannelGroup);

	if (Groupiter == m_ChannelGroups.end())
		return E_FAIL;

	FMOD_CHANNEL* pNewChannel = nullptr;

	FMOD_System_PlaySound(m_pSystem, Sounditer->second, Groupiter->second, FALSE, &pNewChannel);
	FMOD_Channel_SetVolume(pNewChannel, fVolume);
	FMOD_Channel_SetMode(pNewChannel, FMOD_LOOP_NORMAL);
	FMOD_System_Update(m_pSystem);

	if (pChannel != nullptr)
	{
		*pChannel = pNewChannel;
	}

	return S_OK;
}
HRESULT CSound_Manager::PlaySoundFile_AddChannel(const wstring& szChannelTag, const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume)
{
	FMOD_CHANNEL* pChannel = nullptr;
	PlaySoundFile(szChannelGroup, strSoundKey, fVolume, &pChannel);
	Add_Channel(szChannelTag, pChannel);
	return S_OK;
}
HRESULT CSound_Manager::PlayBGM_AddChannel(const wstring& szChannelTag, const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume)
{
	FMOD_CHANNEL* pChannel = nullptr;
	PlayBGM(szChannelGroup, strSoundKey, fVolume, &pChannel);
	Add_Channel(szChannelTag, pChannel);
	return S_OK;
}
void CSound_Manager::Set_ChannelGroupVolume(const wstring& szChannelGroup, _float fVolume)
{
	auto& iter = m_ChannelGroups.find(szChannelGroup);

	if (iter == m_ChannelGroups.end())
		return;

	FMOD_ChannelGroup_SetVolume(iter->second, fVolume);
}

_float CSound_Manager::Get_ChannelGroupVolume(const wstring& szChannelGroup)
{
	auto& iter = m_ChannelGroups.find(szChannelGroup);

	if (iter == m_ChannelGroups.end())
		return -1.0f;

	_float fVolume;
	FMOD_ChannelGroup_GetVolume(iter->second, &fVolume);

	return fVolume;
}

void CSound_Manager::Stop_Channel_Sound(FMOD_CHANNEL* pChannel)
{
	if (pChannel == nullptr)
		return;

	FMOD_Channel_Stop(pChannel);
}

void CSound_Manager::Stop_Channel_Sound(const wstring& szChannelTag)
{
	auto& iter = m_Channels.find(szChannelTag);

	if (iter == m_Channels.end())
		return;

	FMOD_BOOL bPlaying = false;
	FMOD_Channel_IsPlaying(iter->second, &bPlaying);

	if (bPlaying == true)
	{
		FMOD_Channel_Stop(iter->second);
	}
}

void CSound_Manager::Set_Channel_Volume(FMOD_CHANNEL* pChannel, _float fVolume)
{
	if (pChannel == nullptr)
		return;

	FMOD_Channel_SetVolume(pChannel, fVolume);
}

_float CSound_Manager::Get_Channel_Volume(FMOD_CHANNEL* pChannel)
{
	if (pChannel == nullptr)
		return -1.0f;

	_float fVolume;
	FMOD_Channel_GetVolume(pChannel, &fVolume);

	return fVolume;
}

void CSound_Manager::Set_Channel_Volume(const wstring& szChannelTag, _float fVolume)
{
	auto& iter = m_Channels.find(szChannelTag);

	if (iter == m_Channels.end())
		return;

	FMOD_Channel_SetVolume(iter->second, fVolume);
}

_float CSound_Manager::Get_Channel_Volume(const wstring& szChannelTag)
{
	auto& iter = m_Channels.find(szChannelTag);

	if (iter == m_Channels.end())
		return -1.0f;

	_float fVolume;

	FMOD_Channel_GetVolume(iter->second, &fVolume);

	return fVolume;
}

_bool CSound_Manager::Is_Channel_Playing(const wstring& szChannelTag)
{
	auto& iter = m_Channels.find(szChannelTag);

	if (iter == m_Channels.end())
		return false;

	FMOD_BOOL bPlaying;

	FMOD_Channel_IsPlaying(iter->second, &bPlaying);

	return bPlaying;
}

_bool CSound_Manager::Is_Channel_Playing(FMOD_CHANNEL* pChannel)
{
	FMOD_BOOL bPlaying;

	FMOD_Channel_IsPlaying(pChannel, &bPlaying);

	return pChannel;
}

void CSound_Manager::Add_Channel(const wstring& szChannelTag, FMOD_CHANNEL* pChannel)
{
	auto& iter = m_Channels.find(szChannelTag);

	if (iter != m_Channels.end())
	{
		Stop_Channel_Sound(iter->second);
	}

	m_Channels[szChannelTag] = pChannel;
}

void CSound_Manager::Stop_SoundAll()
{
	for (auto& iter : m_ChannelGroups)
	{
		FMOD_ChannelGroup_Stop(iter.second);
	}
}


HRESULT CSound_Manager::Add_SoundTrack(const wstring& strSoundTrack, const wstring& strSoundKey)
{
	{
		auto iter = m_Sounds.find(strSoundKey);

		if (iter == m_Sounds.end())
			return E_FAIL;
	}
	
	{
		auto iter = m_SoundTrack.find(strSoundTrack);

		if (iter == m_SoundTrack.end())
		{
			m_SoundTrack.emplace(strSoundTrack, vector<wstring>());
		}
	}

	
	m_SoundTrack[strSoundTrack].push_back(strSoundKey);

	return S_OK;
}

const wstring& CSound_Manager::Get_RandomSoundKey(const wstring& strSoundTrack)
{
	auto iter = m_SoundTrack.find(strSoundTrack);

	if (iter == m_SoundTrack.end())
		return wstring();

	_uint iSoundIndex = rand() % iter->second.size();

	return iter->second[iSoundIndex];
}

HRESULT CSound_Manager::LoadDirectory(const fs::path& strPath)
{
	for (const auto& entry : fs::directory_iterator(strPath))
	{
		if (fs::is_directory(entry))
		{
			LoadDirectory(entry);
		}
		else if (fs::is_regular_file(entry))
		{
			if (L".wav" == entry.path().extension())
			{
				if (FAILED(LoadSoundFile(entry.path())))
				{
					return E_FAIL;
				}
			}
		}
	}

	return S_OK;
}

HRESULT CSound_Manager::LoadSoundFile(const fs::path& strPath)
{
	wstring strFileName = strPath.filename();
	wstring strFullPath = strPath;

	_int iRequiredSize = WideCharToMultiByte(CP_UTF8, 0, strFullPath.c_str(), -1, NULL, 0, NULL, NULL);
	if (iRequiredSize > 0)
	{
		char* szFullPath = new char[iRequiredSize];
		int result = WideCharToMultiByte(CP_UTF8, 0, strFullPath.c_str(), -1, szFullPath, iRequiredSize, NULL, NULL);

		FMOD_SOUND* pSound = nullptr;
		FMOD_RESULT eRes = FMOD_System_CreateSound(m_pSystem, szFullPath, FMOD_DEFAULT, 0, &pSound);

		Safe_Delete_Array(szFullPath);

		if (eRes == FMOD_OK)
			m_Sounds.emplace(strFileName, pSound);
	}

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

void CSound_Manager::Free()
{
	__super::Free();

	for (auto& Pair : m_Sounds)
	{
		FMOD_Sound_Release(Pair.second);
	}
	m_Sounds.clear();

	FMOD_System_Release(m_pSystem);
	FMOD_System_Close(m_pSystem);

}
