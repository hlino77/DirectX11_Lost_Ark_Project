#pragma once
#include "Client_Defines.h"

class CServerSession : public PacketSession
{
public:
	CServerSession();
	~CServerSession() = default;

	USE_LOCK
public:
	
	virtual void OnConnected() override;
	virtual void OnRecvPacket(BYTE* buffer, int32 len) override;
	virtual void OnSend(int32 len) override;
	virtual void OnDisconnected() override;



	LEVELSTATE	Get_LevelState();
	void		Set_LevelState(LEVELSTATE eState);
private:
	LEVELSTATE m_eLevelState;
};

