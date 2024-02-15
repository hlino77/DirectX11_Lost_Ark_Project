#include "stdafx.h"
#include "ServerPacketHandler.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "GameInstance.h"
#include "Player_Server.h"
#include "AsUtils.h"
#include "Struct.pb.h"
#include "Protocol.pb.h"
#include "ColliderSphere.h"
#include "LobbyUser_Server.h"
#include "ColliderOBB.h"
#include "EventMgr.h"
#include "ServerEvent.h"
#include "Skill_Server.h"
#include "Monster_Server.h"
#include "LevelControlManager.h"
#include "PartyManager.h"
#include "Npc_Server.h"
#include "NavigationMgr.h"
#include "Skill_RisingSun_Server.h"
#include "Skill_TeleportDoor_Server.h"

PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID_Server(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handle_S_TIME_Server(PacketSessionRef& session, Protocol::S_TIME& pkt)
{
	CGameSessionManager::TIME tServerTime = CGameSessionManager::GetInstance()->Get_ServerTime();

	pkt.set_iserverminute(tServerTime.iMinute);
	pkt.set_fserversecond(tServerTime.fSecond);
	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);

	session->Send(pSendBuffer);

	return true;
}

bool Handle_S_LOGIN_Server(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	shared_ptr<CGameSession> pGameSession = dynamic_pointer_cast<CGameSession>(session);

	CLevelControlManager::GetInstance()->Login_Player(pGameSession, pkt);

	return true;
}

bool Handel_S_OPENLEVEL_Server(PacketSessionRef& session, Protocol::S_OPEN_LEVEL& pkt)
{
	return true;
}

bool Handel_S_LEVELSTATE_Server(PacketSessionRef& session, Protocol::S_LEVEL_STATE& pkt)
{
	shared_ptr<CGameSession> pGameSession = dynamic_pointer_cast<CGameSession>(session);
	pGameSession->Set_LevelState((LEVELSTATE)pkt.ilevelstate());

	return true;
}

bool Handel_S_CREATEOBJECT_Server(PacketSessionRef& session, Protocol::S_CREATE_OBJCECT& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/*switch ((OBJ_TYPE)pkt.iobjecttype())
	{
	case OBJ_TYPE::PLAYER:
	{
		CPlayer_Server::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(pkt.strname());

		if (nullptr == pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), TEXT("Prototype_GameObject_Player"), &Desc))
			return E_FAIL;

		break;
	}
	case OBJ_TYPE::MONSTER:
		break;
	case OBJ_TYPE::PROP:
		break;
	}*/

	Safe_Release(pGameInstance);

	return true;
}



bool Handel_S_ANIMATION_Server(PacketSessionRef& session, Protocol::S_ANIMATION& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	CModel* pModel = dynamic_cast<CModel*>(pObject->Get_Component(L"Com_Model"));

	pModel->Reserve_NextAnimation(pkt.ianimindex(), pkt.fchangetime(), pkt.istartframe(), pkt.ichangeframe());

	Safe_Release(pGameInstance);


	return true;
}

bool Handel_S_OBJECTINFO_Server(PacketSessionRef& session, Protocol::S_OBJECTINFO& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	auto tObject = pkt.mutable_tobject(0);


	CGameObject* pObject = pGameInstance->Find_GameObject(tObject->ilevel(), tObject->ilayer(), tObject->iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	pObject->Set_TargetPos(Vec3(tObject->mutable_vtargetpos()->mutable_data()));
	pObject->Get_TransformCom()->Set_WorldMatrix(Matrix(tObject->mutable_matworld()->mutable_data()));


	//CGameSessionManager::GetInstance()->Add_SendCount();
	//cout << CGameSessionManager::GetInstance()->Get_SendCount() << endl;

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_STATE_Server(PacketSessionRef& session, Protocol::S_STATE& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	auto tObject = pkt.tobject();
	

	CGameObject* pObject = pGameInstance->Find_GameObject(tObject.ilevel(), tObject.ilayer(), tObject.iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	Vec3 vTargetPos(tObject.vtargetpos().data());
	Matrix matTargetWorld(tObject.matworld().data());


	pObject->Set_TargetPos(vTargetPos);
	pObject->Set_TargetMatrix(matTargetWorld);
	pObject->Set_ServerState(CAsUtils::ToWString(pkt.strstate()));
	pObject->Set_WeaponIndex(pkt.iweaponindex());

	if (pkt.itargetobjectid() == -1)
		pObject->Reset_NearTarget();
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObject(tObject.ilevel(), pkt.itargetobjectlayer(), pkt.itargetobjectid());
		if (pNearTarget == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		pObject->Set_NearTarget(pNearTarget);
	}


	if (pkt.ihitobjectid() == -1)
		pObject->Reset_HitObject();
	else
	{
		CGameObject* pHitObject = pGameInstance->Find_GameObject(tObject.ilevel(), pkt.ihitobjectlayer(), pkt.ihitobjectid());
		if (pHitObject == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		pObject->Set_HitObject(pHitObject);
	}


	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast_Others(pBuffer, session->GetSessionID());

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_COLLIDERSTATE_Server(PacketSessionRef& session, Protocol::S_COLLIDERSTATE& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	CSphereCollider* pCollider = pObject->Get_Colider(pkt.icollayer());

	pCollider->SetActive(pkt.bactive());
	pCollider->Set_Radius(pkt.fradius());
	pCollider->Set_Offset(Vec3(pkt.voffset().data()));
	pCollider->Update_Collider();
	pCollider->Set_AttackCollider(pkt.iattack(), pkt.iattacktype(), pkt.bslow());

	if (pkt.tchild_size() > 0)
	{
		auto& tChild = pkt.tchild(0);
		COBBCollider* pChild = dynamic_cast<COBBCollider*>(pCollider->Get_Child());
		
		pChild->Set_Offset(Vec3(tChild.voffset().data()));
		pChild->Set_Scale(Vec3(tChild.vscale().data()));
	}

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_COLLISION_Server(PacketSessionRef& session, Protocol::S_COLLISION& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance); 
		return true;
	}

	pObject->Hit_Collision((_uint)pkt.idamage(), Vec3(pkt.vhitpos().data()), pkt.istatuseffect(), pkt.fforce(), pkt.fduration(), pkt.igroggy());
	
	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_USERINFO_Server(PacketSessionRef& session, Protocol::S_USERINFO& pkt)
{

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_NEARTARGET_Server(PacketSessionRef& session, Protocol::S_NEARTARGET& pkt)
{
	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast_Others(pBuffer, session->GetSessionID());

	return true;
}

bool Handel_S_SLOWMOTION_Server(PacketSessionRef& session, Protocol::S_SLOWMOTION& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast_Others(pBuffer, session->GetSessionID());


	Safe_Release(pGameInstance);
	return true;
}


bool Handel_S_EVENT_Server(PacketSessionRef& session, Protocol::S_EVENT& pkt)
{
	dynamic_cast<CServerEvent*>(CEventMgr::GetInstance()->Get_Event(pkt.ieventid()))->Set_ClientState(session->GetSessionID(), (EVENTSTATE)pkt.istate());

	return true;
}

bool Handel_S_HP_Server(PacketSessionRef& session, Protocol::S_HP& pkt)
{
	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast_Others(pBuffer, session->GetSessionID());

	return true;
}

bool Handel_S_CREATEPLAYER_Server(PacketSessionRef& session, Protocol::S_CREATE_PLAYER& pkt)
{



	return true;
}

bool Handel_S_CHAT_Server(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	//CGameSessionManager::GetInstance()->Broadcast_Others(pBuffer, session->GetSessionID());
	CGameSessionManager::GetInstance()->Broadcast(pBuffer);

	return true;
}

bool Handel_S_MONSTERSTATE_Server(PacketSessionRef& session, Protocol::S_MONSTERSTATE& pkt)
{

	return true;
}

bool Handel_S_PLAYERLEVELMOVE_Server(PacketSessionRef& session, Protocol::S_PLAYERLEVELMOVE& pkt)
{
	shared_ptr<CGameSession> pGameSession = dynamic_pointer_cast<CGameSession>(session);

	CLevelControlManager::GetInstance()->Reserve_LevelMove(pGameSession, (LEVELID)pkt.icurrlevel(), (LEVELID)pkt.inextlevel());

	return true;
}

bool Handel_S_DELETEGAMEOBJECT_Server(PacketSessionRef& session, Protocol::S_DELETEGAMEOBJECT& pkt)
{

	return true;
}

bool Handel_S_IDENTITY_Server(PacketSessionRef& session, Protocol::S_IDENTITY& pkt)
{
	SendBufferRef pBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pBuffer);


	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pObject->Set_WeaponIndex(pkt.iweaponindex());

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_PARTY_Server(PacketSessionRef& session, Protocol::S_PARTY& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (pkt.tcreateparty().empty() == false)
	{

	}
	else if (pkt.tjoinparty().empty() == false)
	{
		CPartyManager::GetInstance()->JoinParty(pkt);
	}
	else if (pkt.tinvitationparty().empty() == false)
	{
		auto& tInvitation = pkt.tinvitationparty(0);

		CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(tInvitation.tplayers(1).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, tInvitation.tplayers(1).iid()));
		if (pPlayer == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		pPlayer->Get_GameSession()->Send(CServerPacketHandler::MakeSendBuffer(pkt));
	}

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_NPC_Server(PacketSessionRef& session, Protocol::S_NPC& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	vector<CGameObject*> vecNpcs = pGameInstance->Find_GameObjects(LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_NPC);

	for (auto& pNpc : vecNpcs)
	{
		if (pkt.strnpcname() == CAsUtils::W2S(pNpc->Get_ObjectTag()))
		{
			static_cast<CNpc_Server*>(pNpc)->Actice_Npc_Function(pkt.ilevel(), pkt.iplayerid());
		}
	}

	RELEASE_INSTANCE(CGameInstance);

	return true;
}

bool Handel_S_NAVIGATION_Server(PacketSessionRef& session, Protocol::S_NAVIGATION& pkt)
{
	_uint iLevel = pkt.ilevel();
	_bool bActive = pkt.bactive();

	for (auto& CellIndex : pkt.iindex())
	{
		CNavigationMgr::GetInstance()->Set_NaviCell_Active(iLevel, CellIndex, bActive);
	}
	
	return true;

}

bool Handel_S_CREATESKILL_Server(PacketSessionRef& session, Protocol::S_CREATE_SKILL& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch ((SKILL_TYPE)pkt.iskilltype())
	{
	case SKILL_TYPE::SKILL_RISINGSUN:
	{
		_uint iPlayerID = pkt.iplayerid();
		CGameObject* pPlayer = pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID);

		if (pPlayer == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		CSkill_RisingSun_Server::RisingSunDesc tDesc;
		tDesc.iObjectID = g_iObjectID++;
		tDesc.pSkillOwner = pPlayer;

		CGameObject* pObject = pGameInstance->Add_GameObject(pPlayer->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_RisingSun", &tDesc);

		if (pObject == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		pkt.set_iskillid(tDesc.iObjectID);
		CGameSessionManager::GetInstance()->Broadcast(CServerPacketHandler::MakeSendBuffer(pkt));
	}
		break;

	case SKILL_TYPE::SKILL_TELEPORTDOOR:
	{
		_uint iPlayerID = pkt.iplayerid();
		CGameObject* pPlayer = pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID);

		if (pPlayer == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		CSkill_TeleportDoor_Server::RisingSunDesc tDesc;
		tDesc.iObjectID = g_iObjectID++;
		tDesc.pSkillOwner = pPlayer;

		CGameObject* pObject = pGameInstance->Add_GameObject(pPlayer->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_TeleportDoor", &tDesc);

		if (pObject == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		pkt.set_iskillid(tDesc.iObjectID);
		CGameSessionManager::GetInstance()->Broadcast(CServerPacketHandler::MakeSendBuffer(pkt));
	}
		break;
	}

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_BUFFSKILL_Server(PacketSessionRef& session, Protocol::S_BUFF_SKILL& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CSkill_Server* pSkill = dynamic_cast<CSkill_Server*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_SKILL, pkt.iskillid()));

	if (pSkill == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pSkill->Buff_Player(pkt.iplayerid());

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_PLAYERTELEPORT_Server(PacketSessionRef& session, Protocol::S_PLAYERTELEPORT& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iPlayerID = pkt.iplayerid();
	CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));

	if (pPlayer == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	Matrix matWorld(pkt.matrix().data());
	pPlayer->Set_TargetMatrix(matWorld);
	pPlayer->Get_TransformCom()->Set_WorldMatrix(matWorld);
	pPlayer->Set_ServerState(L"Idle");

	CGameSessionManager::GetInstance()->Broadcast_Others(CServerPacketHandler::MakeSendBuffer(pkt), session->GetSessionID());
	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_CHANGEEQUIP_Server(PacketSessionRef& session, Protocol::S_CHANGEEQUIP& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iPlayerID = pkt.iplayerid();
	CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));

	if (pPlayer == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pPlayer->Set_Equips((_int*)pkt.itemcodes().data());
	CGameSessionManager::GetInstance()->Broadcast_Others(CServerPacketHandler::MakeSendBuffer(pkt), session->GetSessionID());

	Safe_Release(pGameInstance);
	return true;
}
