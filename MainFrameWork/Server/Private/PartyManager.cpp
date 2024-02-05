#include "stdafx.h"
#include "PartyManager.h"
#include "GameObject.h"
#include "Party_Server.h"
#include "Player_Server.h"
#include "GameSession.h"
#include "GameInstance.h"

IMPLEMENT_SINGLETON(CPartyManager)

CPartyManager::CPartyManager()
{
}

void CPartyManager::JoinParty(Protocol::S_PARTY& pkt)
{
	WRITE_LOCK;

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	auto& tJoinParty = pkt.tjoinparty(0);

	CPlayer_Server* pPartyLeader = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(tJoinParty.tplayer(0).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, tJoinParty.tplayer(0).iid()));
	if (pPartyLeader == nullptr)
	{
		Safe_Release(pGameInstance);
		return;
	}

	CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(tJoinParty.tplayer(1).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, tJoinParty.tplayer(1).iid()));
	if (pPlayer == nullptr)
	{
		Safe_Release(pGameInstance);
		return;
	}

	CParty_Server* pParty = Get_Party(pPartyLeader->Get_ObjectID());
	if (pParty == nullptr)
	{
		vector<CPlayer_Server*> Players;
		Players.push_back(pPartyLeader);
		Players.push_back(pPlayer);

		m_Parties.emplace(pPartyLeader->Get_ObjectID(), new CParty_Server(Players));
	}
	else
	{
		pParty->Add_Player(pPlayer);
	}


	Safe_Release(pGameInstance);
}

CParty_Server* CPartyManager::Get_Party(_uint iObjectID)
{
	auto iter = m_Parties.find(iObjectID);

	if (iter == m_Parties.end())
	{
		return nullptr;
	}

	return (*iter).second;
}



void CPartyManager::Free()
{
}


