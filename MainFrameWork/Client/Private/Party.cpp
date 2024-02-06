#include "stdafx.h"
#include "Party.h"
#include "Client_Defines.h"
#include "Player.h"

CParty::CParty(vector<_uint> PlayersId)
	: m_PlayersId(PlayersId)
{

}

CParty::~CParty()
{
}

_bool CParty::Add_Player(CGameObject* pPlayer)
{
	if (m_PlayersId.size() >= 4)
		return false;

	m_PlayersId.push_back(pPlayer->Get_ObjectID());

	return true;
}
