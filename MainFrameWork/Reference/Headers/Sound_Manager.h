#pragma once
#include "Base.h"
#include "Lock.h"


BEGIN(Engine)

class CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;


	
public:
	HRESULT Ready_Sound();
	HRESULT	Initialize_LoopChannel(_uint iStart, _uint iEnd);
public:
	HRESULT PlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume);
	HRESULT	PlaySound_Distance(const wstring& strSoundKey, _uint iChannel, _float fVolume, Vec3 vPos, _float fRange);

	HRESULT PlaySoundFile_LoopChannel(const wstring& strSoundKey, _float fVolume);
	HRESULT	PlaySound_Distance_LoopChannel(const wstring& strSoundKey, _float fVolume, Vec3 vPos, _float fRange);

	HRESULT Find_Stop_Sound(const wstring& strSoundKey);


	HRESULT PlayBGM(const wstring& strSoundKey, _uint iChannel, _float fVolume);
	HRESULT StopSound(_uint iChannel);
	HRESULT StopSoundAll();
	HRESULT SetChannelVolume(_uint iChannel, _float fVolume);
	HRESULT CheckPlaySoundFile(const wstring& strSoundKey, _uint iChannel, _float fVolume);


	HRESULT Add_SoundTrack(const wstring& strSoundTrack, const wstring& strSoundKey);
	const wstring& Get_RandomSoundKey(const wstring& strSoundTrack);
private:
	HRESULT LoadSoundFile();

private:
	FMOD_SYSTEM*	m_pSystem = { nullptr };

	FMOD_CHANNEL*	m_pChannelArr[30];

	map<const wstring, FMOD_SOUND*> m_Sounds;


	list<_uint> m_LoopChannelList;
	_uint m_iLoopChannelSize;

	map<wstring, vector<wstring>> m_SoundTrack;

	USE_LOCK
public:
	virtual void Free();

};

END