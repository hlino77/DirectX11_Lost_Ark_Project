#pragma once
#include "Base.h"
#include "Lock.h"
#include <filesystem>

namespace fs = std::filesystem;

BEGIN(Engine)

class ENGINE_DLL CSound_Manager final : public CBase
{
	DECLARE_SINGLETON(CSound_Manager)

private:
	CSound_Manager();
	virtual ~CSound_Manager() = default;

public:
	HRESULT Ready_Sound();
	HRESULT Add_ChannelGroup(const string& strChannelGroupName, _float fVolume);

public:
	HRESULT PlaySoundFile(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume, FMOD_CHANNEL** pChannel = nullptr);
	HRESULT PlayBGM(const wstring& szChannelGroup, const wstring& strSoundKey, _float fVolume, FMOD_CHANNEL** pChannel = nullptr);

	void	Set_ChannelGroupVolume(const wstring& szChannelGroup, _float fVolume);
	_float	Get_ChannelGroupVolume(const wstring& szChannelGroup);

	void	Stop_Channel_Sound(FMOD_CHANNEL* pChannel);
	void	Stop_Channel_Sound(const wstring & szChannelTag);

	void	Set_Channel_Volume(FMOD_CHANNEL * pChannel, _float fVolume);
	_float	Get_Channel_Volume(FMOD_CHANNEL * pChannel);

	void	Set_Channel_Volume(const wstring & szChannelTag, _float fVolume);
	_float	Get_Channel_Volume(const wstring & szChannelTag);

	void	Add_Channel(const wstring & szChannelTag, FMOD_CHANNEL* pChannel);

	void	Stop_SoundAll();

	HRESULT Add_SoundTrack(const wstring& strSoundTrack, const wstring& strSoundKey);
	const wstring& Get_RandomSoundKey(const wstring& strSoundTrack);
private:
	HRESULT LoadDirectory(const fs::path& strPath);
	HRESULT LoadSoundFile(const fs::path& strPath);

private:
	FMOD_SYSTEM*	m_pSystem = { nullptr };

	map<wstring, FMOD_CHANNELGROUP*> m_ChannelGroups;
	map<wstring, vector<wstring>> m_SoundTrack;

	map<wstring, FMOD_SOUND*> m_Sounds;

	map<wstring, FMOD_CHANNEL*> m_Channels;
	USE_LOCK
public:
	virtual void Free();

};

END