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
	void	Create_Party(CGameObject* pLeader);
	_bool	Join_Party(_uint iID, CGameObject* pPlayer);

private:
	atomic<_uint> m_iPartyID = 1;
	unordered_map<_uint, CParty_Server*> m_Parties;

	USE_LOCK
public:
	virtual void Free() override;
};

END