#include "stdafx.h"
#include "Party_Server.h"
#include "GameObject.h"
#include "Player_Server.h"
#include "GameSession.h"


CParty_Server::CParty_Server(_uint iID, CGameObject* pLeader)
	: m_iID(iID)
{
	m_Players.push_back(pLeader);
}

CParty_Server::~CParty_Server()
{
}

_bool CParty_Server::Add_Player(CGameObject* pPlayer)
{
	if (m_Players.size() >= 4)
		return false;

	{
		Protocol::S_PARTY pkt;

		auto tJoinParty = pkt.add_tjoinparty();

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);	

		for (auto& Player : m_Players)
			dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pSendBuffer);

	}

	{
		Protocol::S_PARTY pkt;

		auto tCreateParty = pkt.add_tcreateparty();

		for (auto& Player : m_Players)
		{
			auto tPlayer = tCreateParty->add_tplayers();
			tPlayer->set_iid(Player->Get_ObjectID());
			tPlayer->set_ilevel(Player->Get_CurrLevel());
		}
	
		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
		dynamic_cast<CPlayer_Server*>(pPlayer)->Get_GameSession()->Send(pSendBuffer);
	}

	m_Players.push_back(pPlayer);

	return true;
}
