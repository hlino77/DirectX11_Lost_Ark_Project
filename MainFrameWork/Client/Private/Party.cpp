#include "stdafx.h"
#include "Party.h"
#include "Client_Defines.h"

CParty::CParty(vector<CGameObject*> Players)
	: m_Players(Players)
{

}

CParty::~CParty()
{
}

_bool CParty::Add_Player(CGameObject* pPlayer)
{
	if (m_Players.size() >= 4)
		return false;

	m_Players.push_back(pPlayer);

	return true;
}
