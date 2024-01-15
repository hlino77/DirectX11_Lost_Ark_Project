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

	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

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


	CGameObject* pObject = pGameInstance->Find_GameObejct(tObject->ilevel(), tObject->ilayer(), tObject->iobjectid());
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
	
	

	CGameObject* pObject = pGameInstance->Find_GameObejct(tObject.ilevel(), tObject.ilayer(), tObject.iobjectid());
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
		CGameObject* pNearTarget = pGameInstance->Find_GameObejct(tObject.ilevel(), pkt.itargetobjectlayer(), pkt.itargetobjectid());
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
		CGameObject* pHitObject = pGameInstance->Find_GameObejct(tObject.ilevel(), pkt.ihitobjectlayer(), pkt.ihitobjectid());
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


	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());
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


	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance); 
		return true;
	}

	pObject->Hit_Collision(pkt.idamage(), Vec3(pkt.vhitpos().data()), pkt.istatuseffect(), pkt.fforce(), pkt.fduration(), pkt.igroggy());
	
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

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}


	pObject->Set_Hp(pkt.ihp());

	Safe_Release(pGameInstance);

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

	CLevelControlManager::GetInstance()->Player_LevelMove(pGameSession, (LEVELID)pkt.icurrlevel(), (LEVELID)pkt.inextlevel());

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
	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

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
		auto& tCreateParty = pkt.tcreateparty(0);
		CGameObject* pObject = pGameInstance->Find_GameObejct(tCreateParty.tplayers(0).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, tCreateParty.tplayers(0).iid());
		if (pObject == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		CPartyManager::GetInstance()->Create_Party(pObject);
	}
	else if (pkt.tjoinparty().empty() == false)
	{
		auto& tJoinParty = pkt.tjoinparty(0);

	}
	else if (pkt.tinvitationparty().empty() == false)
	{
		auto& tInvitation = pkt.tinvitationparty(0);


	}

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_NPC_Server(PacketSessionRef& session, Protocol::S_NPC& pkt)
{


	return true;
}
