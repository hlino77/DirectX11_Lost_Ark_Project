#include "stdafx.h"
#include "Party_Server.h"
#include "GameObject.h"
#include "Player_Server.h"
#include "GameSession.h"


CParty_Server::CParty_Server(vector<CPlayer_Server*>& Players)
	: m_Players(Players)
{
	Protocol::S_PARTY pkt;
	auto tParty = pkt.add_tcreateparty();

	for (auto& Player : Players)
	{
		auto tPlayer = tParty->add_tplayers();
		tPlayer->set_iid(Player->Get_ObjectID());
		tPlayer->set_ilevel(Player->Get_CurrLevel());
	}


	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	for (auto& Player : Players)
	{
		Player->Get_GameSession()->Send(pSendBuffer);
	}
}


CParty_Server::~CParty_Server()
{
}

void CParty_Server::Add_Player(CPlayer_Server* pPlayer)
{
	if (m_Players.size() >= 4)
	{
		return;
	}

	Protocol::S_PARTY CreatePkt;
	auto tCreateParty = CreatePkt.add_tcreateparty();

	{
		Protocol::S_PARTY JoinPkt;
		auto tJoinParty = JoinPkt.add_tjoinparty();
		auto tJoinPlayer = tJoinParty->add_tplayer();
		tJoinPlayer->set_iid(pPlayer->Get_ObjectID());
		tJoinPlayer->set_ilevel(pPlayer->Get_CurrLevel());

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(JoinPkt);

		for (auto& Player : m_Players)
		{
			auto tPlayer = tCreateParty->add_tplayers();
			tPlayer->set_iid(Player->Get_ObjectID());
			tPlayer->set_ilevel(Player->Get_CurrLevel());
			Player->Get_GameSession()->Send(pSendBuffer);
		}
	}

	m_Players.push_back(pPlayer);
	auto tPlayer = tCreateParty->add_tplayers();
	tPlayer->set_iid((pPlayer->Get_ObjectID()));
	tPlayer->set_ilevel(pPlayer->Get_CurrLevel());
	pPlayer->Get_GameSession()->Send(CServerPacketHandler::MakeSendBuffer(CreatePkt));
}

