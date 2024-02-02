#include "stdafx.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "LevelControlManager.h"
#include "GameInstance.h"
#include "Player_Server.h"
#include "AsUtils.h"
#include "ItemCode.h"



IMPLEMENT_SINGLETON(CLevelControlManager)

CLevelControlManager::CLevelControlManager()
{
}


HRESULT CLevelControlManager::Login_Player(shared_ptr<CGameSession>& pGameSession, Protocol::S_LOGIN& pkt)
{
	WRITE_LOCK

	_uint iLevel = LEVELID::LEVEL_BERN;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	vector<CGameObject*>& CurrLevelPlayers = CGameInstance::GetInstance()->Find_GameObjects(iLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);

	for (auto& Player : CurrLevelPlayers)
	{
		CPlayer_Server* pOtherPlayer = dynamic_cast<CPlayer_Server*>(Player);

		Protocol::S_CREATE_PLAYER tPlayerPkt;

		tPlayerPkt.set_iobjectid(pOtherPlayer->Get_ObjectID());
		tPlayerPkt.set_iclass(pOtherPlayer->Get_Class());
		tPlayerPkt.set_bcontroll(false);
		tPlayerPkt.set_strnickname(CAsUtils::W2S(pOtherPlayer->Get_NickName()));
		tPlayerPkt.set_strstate(CAsUtils::ToString(pOtherPlayer->Get_ServerState()));
		tPlayerPkt.set_ilevel(iLevel);
		tPlayerPkt.set_iweaponindex(pOtherPlayer->Get_WeaponIndex());

		auto ItemCodes = tPlayerPkt.mutable_itemcodes();
		ItemCodes->Resize((_uint)ITEMPART::_END, -1);
		memcpy(ItemCodes->mutable_data(), pOtherPlayer->Get_Equips(), sizeof(_int) * (_uint)ITEMPART::_END);

		auto vPktTargetPos = tPlayerPkt.mutable_vtargetpos();
		vPktTargetPos->Resize(3, 0.0f);
		Vec3 vTargetPos = pOtherPlayer->Get_TargetPos(); 
		memcpy(vPktTargetPos->mutable_data(), &vTargetPos, sizeof(Vec3));


		auto vPacketWorld = tPlayerPkt.mutable_matworld();
		vPacketWorld->Resize(16, 0.0f);
		Matrix matOtherWorld = pOtherPlayer->Get_TransformCom()->Get_WorldMatrix();
		memcpy(vPacketWorld->mutable_data(), &matOtherWorld, sizeof(Matrix));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tPlayerPkt);
		pGameSession->Send(pSendBuffer);
	}

	pGameSession->Set_Class(pkt.iclass());
	pGameSession->Set_NickName(CAsUtils::S2W(pkt.strnickname()));

	{
		vector<_int> m_Equips;
		m_Equips.resize((_uint)ITEMPART::_END, -1);
		Compute_DefaultEquipCodes(pkt.iclass(), m_Equips);

		CPlayer_Server::MODELDESC Desc;
		Desc.iWeaponIndex = -1;

		switch (pkt.iclass())
		{
		case (_uint)CHR_CLASS::GUNSLINGER:
			Desc.strFileName = L"Gunslinger";
			Desc.iWeaponIndex = 0;
			break;
		case (_uint)CHR_CLASS::SLAYER:
			Desc.strFileName = L"WR";
			break;
		case (_uint)CHR_CLASS::DESTROYER:
			Desc.strFileName = L"WDR";
			break;
		case (_uint)CHR_CLASS::BARD:
			Desc.strFileName = L"MG";
			break;
		case (_uint)CHR_CLASS::DOAGA:
			Desc.strFileName = L"SP";
			break;
		}
		Desc.iObjectID = g_iObjectID++;
		pGameSession->Set_PlayerID(Desc.iObjectID);

		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_PLAYER;
		Desc.pGameSession = pGameSession;
		Desc.iClass = pGameSession->Get_Class();
		Desc.szNickName = pGameSession->Get_NickName();
		Desc.iCurrLevel = iLevel;
		Desc.pItemCodes = m_Equips.data();

		Matrix matWorld = XMMatrixIdentity();
		matWorld.Translation(Get_LevelSpawnPos((LEVELID)iLevel));

		CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Add_GameObject(iLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Prototype_GameObject_Player"), &Desc));
		if (nullptr == pPlayer)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		pPlayer->Get_TransformCom()->Set_WorldMatrix(matWorld);

		{
			Protocol::S_CREATE_PLAYER tPlayerPkt;

			tPlayerPkt.set_iobjectid(Desc.iObjectID);
			tPlayerPkt.set_iclass(pkt.iclass());
			tPlayerPkt.set_bcontroll(true);
			tPlayerPkt.set_strnickname(pkt.strnickname());
			tPlayerPkt.set_strstate("Idle");
			tPlayerPkt.set_ilevel(iLevel);
			tPlayerPkt.set_iweaponindex(Desc.iWeaponIndex);

			auto ItemCodes = tPlayerPkt.mutable_itemcodes();
			ItemCodes->Resize((_uint)ITEMPART::_END, -1);
			memcpy(ItemCodes->mutable_data(), pPlayer->Get_Equips(), sizeof(_int) * (_uint)ITEMPART::_END);

			auto vPktTargetPos = tPlayerPkt.mutable_vtargetpos();
			vPktTargetPos->Resize(3, 0.0f);
			Vec3 vTargetPos = pPlayer->Get_TargetPos();
			memcpy(vPktTargetPos->mutable_data(), &vTargetPos, sizeof(Vec3));

			auto vPacketWorld = tPlayerPkt.mutable_matworld();
			vPacketWorld->Resize(16, 0.0f);
			memcpy(vPacketWorld->mutable_data(), &matWorld, sizeof(Matrix));

			SendBufferRef pOwnerSendBuffer = CServerPacketHandler::MakeSendBuffer(tPlayerPkt);
			pGameSession->Send(pOwnerSendBuffer);


			tPlayerPkt.set_bcontroll(false);
			SendBufferRef pOtherSendBuffer = CServerPacketHandler::MakeSendBuffer(tPlayerPkt);

			for (auto& Player : CurrLevelPlayers)
			{
				if(Player != pPlayer)
					dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pOtherSendBuffer);
			}
		}
	}

	Safe_Release(pGameInstance);

	return S_OK;
}


HRESULT CLevelControlManager::Player_LevelMove(shared_ptr<CGameSession>& pOwnerSession, _uint iCurrLevel, _uint iNextLevel)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, pOwnerSession->Get_PlayerID()));

	vector<CGameObject*>& CurrLevelPlayers = CGameInstance::GetInstance()->Find_GameObjects(iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);

	{
		Protocol::S_DELETEGAMEOBJECT tDeletePkt;	

		tDeletePkt.set_ilevel(iCurrLevel);
		tDeletePkt.set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);
		tDeletePkt.set_iobjectid(pPlayer->Get_ObjectID());

		SendBufferRef pDeleteBuffer = CServerPacketHandler::MakeSendBuffer(tDeletePkt);


		for (auto& Player : CurrLevelPlayers)
			dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pDeleteBuffer);

	}

	Matrix matWorld = XMMatrixIdentity();
	Vec3 vScale = pPlayer->Get_TransformCom()->Get_Scale();
	matWorld.Translation(Get_LevelSpawnPos((LEVELID)iNextLevel));
	pPlayer->Get_TransformCom()->Set_WorldMatrix(matWorld);
	pPlayer->Get_TransformCom()->Set_Scale(vScale);


	{
		Safe_AddRef(pPlayer);
		pGameInstance->Delete_GameObject(iCurrLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, pPlayer);
		pGameInstance->Add_GameObject(iNextLevel, (_uint)LAYER_TYPE::LAYER_PLAYER, pPlayer);
	}

	pPlayer->Set_CurrLevel(iNextLevel);
	vector<CGameObject*> NextLevelPlayers = pGameInstance->Find_GameObjects(iNextLevel, (_uint)LAYER_TYPE::LAYER_PLAYER);

	for (auto& Player : NextLevelPlayers)
	{
		if (Player == pPlayer)
			continue;

		CPlayer_Server* pOtherPlayer = dynamic_cast<CPlayer_Server*>(Player);

		Protocol::S_CREATE_PLAYER tPlayerPkt;

		tPlayerPkt.set_iobjectid(pOtherPlayer->Get_ObjectID());
		tPlayerPkt.set_iclass(pOtherPlayer->Get_Class());
		tPlayerPkt.set_bcontroll(false);
		tPlayerPkt.set_strnickname(CAsUtils::W2S(pOtherPlayer->Get_NickName()));
		tPlayerPkt.set_strstate(CAsUtils::ToString(pOtherPlayer->Get_ServerState()));
		tPlayerPkt.set_ilevel(iNextLevel);
		tPlayerPkt.set_iweaponindex(pOtherPlayer->Get_WeaponIndex());

		auto ItemCodes = tPlayerPkt.mutable_itemcodes();
		ItemCodes->Resize((_uint)ITEMPART::_END, -1);
		memcpy(ItemCodes->mutable_data(), pOtherPlayer->Get_Equips(), sizeof(_int) * (_uint)ITEMPART::_END);

		auto vPktTargetPos = tPlayerPkt.mutable_vtargetpos();
		vPktTargetPos->Resize(3, 0.0f);
		Vec3 vTargetPos = pOtherPlayer->Get_TargetPos();
		memcpy(vPktTargetPos->mutable_data(), &vTargetPos, sizeof(Vec3));

		auto vPacketWorld = tPlayerPkt.mutable_matworld();
		vPacketWorld->Resize(16, 0.0f);
		Matrix matOtherWorld = pOtherPlayer->Get_TransformCom()->Get_WorldMatrix();
		memcpy(vPacketWorld->mutable_data(), &matOtherWorld, sizeof(Matrix));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tPlayerPkt);
		pOwnerSession->Send(pSendBuffer);
	}


	{

		{
			Protocol::S_CREATE_PLAYER tPlayerPkt;

			tPlayerPkt.set_iobjectid(pPlayer->Get_ObjectID());
			tPlayerPkt.set_iclass(pPlayer->Get_Class());
			tPlayerPkt.set_bcontroll(true);
			tPlayerPkt.set_strnickname(CAsUtils::W2S(pPlayer->Get_NickName()));
			tPlayerPkt.set_strstate("Idle");
			tPlayerPkt.set_ilevel(iNextLevel);
			tPlayerPkt.set_iweaponindex(pPlayer->Get_WeaponIndex());

			auto ItemCodes = tPlayerPkt.mutable_itemcodes();
			ItemCodes->Resize((_uint)ITEMPART::_END, -1);
			memcpy(ItemCodes->mutable_data(), pPlayer->Get_Equips(), sizeof(_int) * (_uint)ITEMPART::_END);

			auto vPktTargetPos = tPlayerPkt.mutable_vtargetpos();
			vPktTargetPos->Resize(3, 0.0f);
			Vec3 vTargetPos = pPlayer->Get_TargetPos();
			memcpy(vPktTargetPos->mutable_data(), &vTargetPos, sizeof(Vec3));

			auto vPacketWorld = tPlayerPkt.mutable_matworld();
			vPacketWorld->Resize(16, 0.0f);
			memcpy(vPacketWorld->mutable_data(), &matWorld, sizeof(Matrix));

			tPlayerPkt.set_bcontroll(false);
			SendBufferRef pOtherSendBuffer = CServerPacketHandler::MakeSendBuffer(tPlayerPkt);

			for (auto& Player : NextLevelPlayers)
			{
				if (Player != pPlayer)
					dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pOtherSendBuffer);
			}
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;

}

void CLevelControlManager::Compute_DefaultEquipCodes(_uint iClass, vector<_int>& pItemCodes)
{
	switch (iClass)
	{
	case (_uint)CHR_CLASS::GUNSLINGER:
		pItemCodes[(_uint)ITEMPART::HELMET] = (_int)ITEMCODE::GN_Helmet_Mococo;
		pItemCodes[(_uint)ITEMPART::BODY] = (_int)ITEMCODE::GN_Body_Mococo;
		pItemCodes[(_uint)ITEMPART::WEAPON] = (_int)ITEMCODE::GN_WP_Mococo;
		break;
	case (_uint)CHR_CLASS::SLAYER:
		pItemCodes[(_uint)ITEMPART::HELMET] = (_int)ITEMCODE::WR_Helmet_Mococo;
		pItemCodes[(_uint)ITEMPART::BODY] = (_int)ITEMCODE::WR_Body_Mococo;
		pItemCodes[(_uint)ITEMPART::WEAPON] = (_int)ITEMCODE::WR_WP_Mococo;
		break;
	case (_uint)CHR_CLASS::DESTROYER:
		pItemCodes[(_uint)ITEMPART::HELMET] = (_int)ITEMCODE::WDR_Helmet_Mococo;
		pItemCodes[(_uint)ITEMPART::BODY] = (_int)ITEMCODE::WDR_Body_Mococo;
		pItemCodes[(_uint)ITEMPART::WEAPON] = (_int)ITEMCODE::WDR_WP_Mococo;
		break;
	case (_uint)CHR_CLASS::BARD:
		break;
	case (_uint)CHR_CLASS::DOAGA:
		pItemCodes[(_uint)ITEMPART::HELMET] = (_int)ITEMCODE::SP_Helmet_Dino;
		pItemCodes[(_uint)ITEMPART::BODY] = (_int)ITEMCODE::SP_Body_Dino;
		pItemCodes[(_uint)ITEMPART::WEAPON] = (_int)ITEMCODE::SP_WP_Dino;
		break;
	}
}



void CLevelControlManager::Update_LevelMove()
{
	WRITE_LOCK
	for (auto Desc : m_LevelMoveList)
	{
		Player_LevelMove(Desc.pSession, Desc.iCurrLevel, Desc.iNextLevel);
	}
	m_LevelMoveList.clear();
}

HRESULT CLevelControlManager::Reserve_LevelMove(shared_ptr<CGameSession>& pOwnerSession, _uint iCurrLevel, _uint iNextLevel)
{
	LEVELMOVEDESC tDesc;
	tDesc.pSession = pOwnerSession;
	tDesc.iCurrLevel = iCurrLevel;
	tDesc.iNextLevel = iNextLevel;

	WRITE_LOCK;
	m_LevelMoveList.push_back(tDesc);

	return S_OK;
}

void CLevelControlManager::Send_LevelState(shared_ptr<CGameSession>& pSession, _uint iLevelState)
{
	Protocol::S_LEVEL_STATE pkt;
	pkt.set_ilevelstate(iLevelState);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	pSession->Send(pSendBuffer);
}

Vec3 CLevelControlManager::Get_LevelSpawnPos(LEVELID eLevel)
{
	Vec3 vPos;

	switch (eLevel)
	{
	case LEVELID::LEVEL_BERN:
		vPos = Vec3(90.63f, 0.0f, 99.44f);
		break;
	case LEVELID::LEVEL_CHAOS_1:
		vPos = Vec3(117.93f, 0.19f, 100.2f);
		break;
	case LEVELID::LEVEL_CHAOS_2:
		vPos = Vec3(100.f, 0.19f, 100.f);
		break;
	case LEVELID::LEVEL_CHAOS_3:
		vPos = Vec3(100.f, 0.19f, 98.f);
		break;
	case LEVELID::LEVEL_VALTANMAIN:
		vPos = Vec3(100.f, 0.19f, 100.f);
		break;

	}


	return vPos;
}




void CLevelControlManager::Free()
{
}


