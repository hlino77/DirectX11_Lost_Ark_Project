#pragma once
#include "Session.h"
#include "Server_Defines.h"

BEGIN(Server)

class CGameSession : public PacketSession
{
public:
	CGameSession();
	~CGameSession() = default;

	USE_LOCK
public:

	virtual void OnConnected() override;
	virtual void OnDisconnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;


	LEVELSTATE	Get_LevelState();
	void		Set_LevelState(LEVELSTATE eState);

	_uint		Get_PlayerID();
	void		Set_PlayerID(_uint iID);

	const _uint		Get_Class();
	void			Set_Class(const _uint iClass);

	const wstring	Get_NickName();
	void			Set_NickName(const wstring& strName);
private:
	LEVELSTATE	m_eLevelState;
	_int		m_iClass = -1;
	wstring		m_strNickName;
	_int		m_iPlayerID = -1;
};

END