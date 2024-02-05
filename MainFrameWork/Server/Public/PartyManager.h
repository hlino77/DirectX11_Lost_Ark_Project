#pragma once
#include "Base.h"
#include "Lock.h"


BEGIN(Engine)
class CGameObject;
END

BEGIN(Server)
class CParty_Server;

class CPartyManager : public CBase
{
	DECLARE_SINGLETON(CPartyManager);

public:
	explicit CPartyManager();
	virtual ~CPartyManager() = default;

public:
	void	JoinParty(Protocol::S_PARTY& pkt);

	CParty_Server* Get_Party(_uint iObjectID);

private:
	unordered_map<_uint, CParty_Server*> m_Parties;

	USE_LOCK
public:
	virtual void Free() override;
};

END