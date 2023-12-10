#include "Sound_Manager.h"
#include "PipeLine.h"
#include "AsUtils.h"


IMPLEMENT_SINGLETON(CSound_Manager)

CSound_Manager::CSound_Manager()
{
	ZeroMemory(m_pChannelArr, sizeof(m_pChannelArr));
}

HRESULT CSound_Manager::Ready_Sound()
{
	FMOD_System_Create(&m_pSystem, FMOD_VERSION);

	FMOD_System_Init(m_pSystem, 30, FMOD_INIT_NORMAL, NULL);

	if (nullptr == m_pSystem)
		return E_FAIL;

	if (FAILED(LoadSoundFile()))
		return E_FAIL;

	return S_OK;
}

HRESULT CSound_Manager::Initialize_LoopChannel(_uint iStart, _uint iEnd)
{
	for (_uint i = iStart; i <= iEnd; ++i)
		m_LoopChannelList.push_back(i);


	m_iLoopChannelSize = m_LoopChannelList.size();

	return S_OK;
}

HRESULT CSound_Manager::PlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	auto iter = m_Sounds.find(strSoundKey);

	if (iter == m_Sounds.end())
		return E_FAIL;

	FMOD_Channel_Stop(m_pChannelArr[iChannel]);
	FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iChannel]);

	FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSound_Manager::PlaySound_Distance(const wstring& strSoundKey, _uint iChannel, _float fVolume, Vec3 vPos, _float fRange)
{
	Vec3 vCamPos = CPipeLine::GetInstance()->Get_CamPosition();

	_float fDistance = (vCamPos - vPos).Length();

	if (fDistance > fRange)
		return S_OK;

	_float fDistanceVolume = min(fVolume * (1.0f - (fDistance / fRange)) * 1.25f, fVolume);
	
	return PlaySoundFile(strSoundKey, iChannel, fDistanceVolume);
}

HRESULT CSound_Manager::PlaySoundFile_LoopChannel(const wstring& strSoundKey, _float fVolume)
{
	WRITE_LOCK

	auto iter = m_Sounds.find(strSoundKey);

	if (iter == m_Sounds.end())
		return E_FAIL;


	for (_uint i = 0; i < m_iLoopChannelSize; ++i)
	{
		_uint iChannel = m_LoopChannelList.front();
		m_LoopChannelList.pop_front();
		m_LoopChannelList.push_back(iChannel);


		FMOD_BOOL    bPlay = FALSE;
		FMOD_RESULT bResult = FMOD_Channel_IsPlaying(m_pChannelArr[iChannel], &bPlay);

		if (bPlay == FALSE)
		{
			FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iChannel]);
			FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);
			FMOD_System_Update(m_pSystem);

			return S_OK;
		}
	}

	_uint iChannel = m_LoopChannelList.front();
	m_LoopChannelList.pop_front();
	m_LoopChannelList.push_back(iChannel);

	FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iChannel]);
	FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);
	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSound_Manager::PlaySound_Distance_LoopChannel(const wstring& strSoundKey, _float fVolume, Vec3 vPos, _float fRange)
{

	Vec3 vCamPos = CPipeLine::GetInstance()->Get_CamPosition();

	_float fDistance = (vCamPos - vPos).Length();


	if (fDistance > fRange)
		return S_OK;

	_float fDistanceVolume = min(fVolume * (1.0f - (fDistance / fRange)) * 1.25f, fVolume);
	

	return PlaySoundFile_LoopChannel(strSoundKey, fDistanceVolume);
}

HRESULT CSound_Manager::Find_Stop_Sound(const wstring& strSoundKey)
{
	string strSoundName = CAsUtils::ToString(strSoundKey);

	for (auto& ChannelIndex : m_LoopChannelList)
	{
		FMOD_SOUND* pSound = nullptr;
		if (FMOD_Channel_GetCurrentSound(m_pChannelArr[ChannelIndex], &pSound) == FMOD_OK)
		{
			char szName[MAX_PATH];
			FMOD_Sound_GetName(pSound, szName, MAX_PATH);

			if (strSoundName == szName)
				FMOD_Channel_Stop(m_pChannelArr[ChannelIndex]);
		}
	}

	return S_OK;
}


HRESULT CSound_Manager::PlayBGM(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	if (nullptr == m_pSystem)
		return E_FAIL;

	auto iter = m_Sounds.find(strSoundKey);

	if (iter == m_Sounds.end())
		return E_FAIL;

	FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iChannel]);
	FMOD_Channel_SetMode(m_pChannelArr[iChannel], FMOD_LOOP_NORMAL);
	FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);
	FMOD_System_Update(m_pSystem);

	return S_OK;
} 

HRESULT CSound_Manager::StopSound(_uint iChannel)
{
	if (nullptr == m_pSystem)
		return E_FAIL;

	FMOD_Channel_Stop(m_pChannelArr[iChannel]);

	return S_OK;
}

HRESULT CSound_Manager::StopSoundAll()
{
	if (nullptr == m_pSystem)
		return E_FAIL;

	for (auto iter : m_pChannelArr)
		FMOD_Channel_Stop(iter);

	return S_OK;
}

HRESULT CSound_Manager::SetChannelVolume(_uint iChannel, _float fVolume)
{
	if (nullptr == m_pSystem)
		return E_FAIL;

	FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);

	FMOD_System_Update(m_pSystem);

	return S_OK;
}

HRESULT CSound_Manager::CheckPlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume)
{
	auto iter = m_Sounds.find(strSoundKey);

	if (iter == m_Sounds.end())
		return E_FAIL;

	FMOD_BOOL    bPlay = FALSE;
	FMOD_RESULT bResult = FMOD_Channel_IsPlaying(m_pChannelArr[iChannel], &bPlay);

	if (bResult != FMOD_OK)
	{
		FMOD_Channel_Stop(m_pChannelArr[iChannel]);
		FMOD_System_PlaySound(m_pSystem, iter->second, NULL, FALSE, &m_pChannelArr[iChannel]);
	}

	FMOD_Channel_SetVolume(m_pChannelArr[iChannel], fVolume);

	FMOD_System_Update(m_pSystem);

	return S_OK;
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

HRESULT CSound_Manager::LoadSoundFile()
{
	_finddata_t fd;

	long long handle = _findfirst("../Bin/Resources/Sounds/*.*", &fd);

	if (handle == -1)
		return E_FAIL;

	_int iResult = 0;

	wstring strCurPath = TEXT("../Bin/Resources/Sounds/");
	
	while (iResult != -1)
	{
		wstring strFileName = wstring(fd.name, fd.name + strlen(fd.name));
		wstring strFullPath = strCurPath + strFileName;

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

		iResult = _findnext(handle, &fd);
	}

	FMOD_System_Update(m_pSystem);

	_findclose(handle);

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
