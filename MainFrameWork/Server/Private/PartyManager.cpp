#include "stdafx.h"
#include "PartyManager.h"
#include "GameObject.h"
#include "Party_Server.h"
#include "Player_Server.h"
#include "GameSession.h"

IMPLEMENT_SINGLETON(CPartyManager)

CPartyManager::CPartyManager()
{
}

void CPartyManager::Create_Party(CGameObject* pLeader)
{
	WRITE_LOCK
	_uint iPartyID = m_iPartyID++;
	m_Parties.emplace(iPartyID, new CParty_Server(iPartyID, pLeader));

	Protocol::S_PARTY pkt;

	auto tCreateParty = pkt.add_tcreateparty();
	auto Player = tCreateParty->add_tplayers();
	Player->set_iid(pLeader->Get_ObjectID());
	Player->set_ilevel(0);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	dynamic_cast<CPlayer_Server*>(pLeader)->Get_GameSession()->Send(pSendBuffer);
}

_bool CPartyManager::Join_Party(_uint iID, CGameObject* pPlayer)
{
	WRITE_LOCK
	if (m_Parties.find(iID) == m_Parties.end())
		return false;

	return m_Parties[iID]->Add_Player(pPlayer);
}



void CPartyManager::Free()
{
}


