#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CParty : public CBase
{
public:
	CParty(_uint iID, vector<CGameObject*> Players);
	~CParty();
public:
	_bool	Add_Player(CGameObject* pPlayer);


private:
	_uint m_iID;
	vector<CGameObject*> m_Players;

	USE_LOCK
};

END