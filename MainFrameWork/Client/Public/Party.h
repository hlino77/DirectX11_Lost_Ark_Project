#pragma once
#include "Base.h"

BEGIN(Engine)
class CGameObject;
END


BEGIN(Client)

class CParty : public CBase
{
public:
	CParty(vector<_uint> PlayersId);
	~CParty();
public:
	_bool	Add_Player(CGameObject* pPlayer);
	vector< _uint>	Get_PartyMembers() { return m_PlayersId; }

	_uint	Get_PartyIndex(CGameObject* pPlayer);
private:
	vector< _uint> m_PlayersId;

	USE_LOCK
};

END