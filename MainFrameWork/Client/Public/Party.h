#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CParty : public CBase
{
public:
	CParty(vector<CGameObject*> Players);
	~CParty();
public:
	_bool	Add_Player(CGameObject* pPlayer);


private:
	vector<CGameObject*> m_Players;

	USE_LOCK
};

END