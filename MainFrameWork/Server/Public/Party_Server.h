#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Server)

class CParty_Server : public CBase
{
public:
	CParty_Server(_uint iID, CGameObject* pLeader);
	~CParty_Server();
public:
	_bool	Add_Player(CGameObject* pPlayer);


private:
	_uint m_iID;
	vector<CGameObject*> m_Players;

	USE_LOCK
};

END