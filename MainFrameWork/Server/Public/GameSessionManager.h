#pragma once
#include <set>

BEGIN(Server)

class CGameSession;

using GameSessionRef = shared_ptr<CGameSession>;

class CGameSessionManager : public CBase
{
	DECLARE_SINGLETON(CGameSessionManager)
public:
	typedef struct SERVERTIME
	{
		_uint iMinute = 0;
		_float fSecond = 0.0f;
	}TIME;


private:
	CGameSessionManager();
	virtual ~CGameSessionManager() = default;

public:
	void Tick(_float fTimeDelta);
public:
	void Add(GameSessionRef session);
	void Remove(GameSessionRef session);
	void Broadcast(SendBufferRef sendBuffer);
	void Broadcast_Others(SendBufferRef sendBuffer, uint32 _ID);

public:
	TIME Get_ServerTime() { return m_tServerTime; }
	uint32 Get_SessionCount();

	set<GameSessionRef>& Get_Sessions() { return _sessions; }

	void				 Add_SendCount() { m_iSendCount.fetch_add(1); }
	uint64				 Get_SendCount() { return m_iSendCount.load(); }
private:
	USE_LOCK;
	set<GameSessionRef> _sessions;

private:
	uint32 m_iSessionID = 0;
	atomic<uint64>  m_iSendCount = 0;
	TIME m_tServerTime;
};

END

