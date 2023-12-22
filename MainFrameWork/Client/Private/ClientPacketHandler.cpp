#include "stdafx.h"
#include "ClientPacketHandler.h"
#include "GameInstance.h"
#include "Client_Defines.h"
#include "Level_Loading.h"
#include "ServerSessionManager.h"
#include "ServerSession.h"
#include "Player.h"
#include "AsUtils.h"
#include "Monster.h"
#include "ColliderSphere.h"
#include "Boss.h"
#include "Skill.h"
#include "Camera_Player.h"
#include "EventMgr.h"
#include "Event.h"
#include "NavigationMgr.h"
#include "Chat_Manager.h"


PacketHandlerFunc GPacketHandler[UINT16_MAX];

bool Handle_INVALID_Client(PacketSessionRef& session, BYTE* buffer, int32 len)
{
	PacketHeader* header = reinterpret_cast<PacketHeader*>(buffer);
	// TODO : Log
	return false;
}

bool Handel_S_TIME_Client(PacketSessionRef& session, Protocol::S_TIME& pkt)
{
	CServerSessionManager::TIME tServerTime, tClientTime;
	CServerSessionManager::GetInstance()->Get_ClientTime();


	LARGE_INTEGER iSendTick, iRecvTick, iCpu;
	
	iSendTick.QuadPart = pkt.isendtick();

	tServerTime.iMinute = pkt.iserverminute();
	tServerTime.fSecond = pkt.fserversecond();

	QueryPerformanceCounter(&iRecvTick);
	QueryPerformanceFrequency(&iCpu);

	uint64 iDelayTick = iRecvTick.QuadPart - iSendTick.QuadPart;
	_float fDelayTime = _float(iDelayTick) / iCpu.QuadPart;

	fDelayTime /= 2.0f;

	tClientTime = tServerTime;
	tClientTime.fSecond += fDelayTime;

	CServerSessionManager::GetInstance()->Set_ClientTime(tClientTime);
	CServerSessionManager::GetInstance()->Set_NetworkDelay(fDelayTime);

	return true;
}

bool Handel_S_LOGIN_Client(PacketSessionRef& session, Protocol::S_LOGIN& pkt)
{
	return false;
}


bool Handel_S_OPENLEVEL_Client(PacketSessionRef& session, Protocol::S_OPEN_LEVEL& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	while (true)
	{
		if ((_uint)LEVELID::LEVEL_LOADING != pGameInstance->Get_CurrLevelIndex())
		{
			if (FAILED(pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(pGameInstance->Get_Device(), pGameInstance->Get_Context(), (LEVELID)pkt.ilevelid(), L"LoadingWnd"))))
			{
				Safe_Release(pGameInstance);
				return true;
			}

			CServerSessionManager::GetInstance()->Get_ServerSession()->Set_LevelState(LEVELSTATE::LOADING);
			Protocol::S_LEVEL_STATE pkt;
			pkt.set_ilevelstate((_uint)LEVELSTATE::LOADING);

			SendBufferRef pSendBuffer = CClientPacketHandler::MakeSendBuffer(pkt);
			CServerSessionManager::GetInstance()->Send(pSendBuffer);
			break;
		}
	}

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_LEVELSTATE_Client(PacketSessionRef& session, Protocol::S_LEVEL_STATE& pkt)
{
	ServerSessionRef& pServerSession = CServerSessionManager::GetInstance()->Get_ServerSession();
	pServerSession->Set_LevelState((LEVELSTATE)pkt.ilevelstate());
	return true;
}

bool Handel_S_CREATEOBJECT_Client(PacketSessionRef& session, Protocol::S_CREATE_OBJCECT& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	switch ((OBJ_TYPE)pkt.iobjecttype())
	{

	case OBJ_TYPE::MONSTER:
	{
		CMonster::MODELDESC Desc;
		Desc.strFileName =L"Monster_"+ CAsUtils::ToWString(pkt.strname());
		Desc.iObjectID = pkt.iobjectid();
		Desc.iLayer = pkt.ilayer();
		Desc.vPos = Vec3(pkt.vpos().data());

		wstring szProtoName = L"Prototype_GameObject_" + Desc.strFileName;
		CMonster* pMonster = dynamic_cast<CMonster*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szProtoName, &Desc));
		if (nullptr == pMonster)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		CNavigationMgr::GetInstance()->Find_FirstCell(pMonster);
		break;
	}
	case OBJ_TYPE::BOSS:
	{
		CBoss::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(pkt.strname());
		Desc.iObjectID = pkt.iobjectid();
		Desc.iLayer = pkt.ilayer();

		wstring szProtoName = L"Prototype_GameObject_" + Desc.strFileName;
		CBoss* pBoss = dynamic_cast<CBoss*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szProtoName, &Desc));
		if (nullptr == pBoss)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		pBoss->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, Vec3(pkt.vpos().data()));
		CNavigationMgr::GetInstance()->Find_FirstCell(pBoss);
		break;
	}
	case OBJ_TYPE::SPAWNER:
	{
		wstring szProtoName = L"Prototype_GameObject_";
		wstring szSpawnerName = CAsUtils::ToWString(pkt.strname());
		szProtoName += szSpawnerName;

		_uint iObjectID = pkt.iobjectid();

		CGameObject* pSpawner = pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szProtoName, &iObjectID);
		if (nullptr == pSpawner)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		pSpawner->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, Vec3(pkt.vpos().data()));
		break;
	}


		
	}

	Safe_Release(pGameInstance);
}


bool Handel_S_ANIMATION_Client(PacketSessionRef& session, Protocol::S_ANIMATION& pkt)
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

bool Handel_S_OBJECTINFO_Client(PacketSessionRef& session, Protocol::S_OBJECTINFO& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return true;
	_uint iUserPlayerID = pPlayer->Get_ObjectID();


	for (_uint i = 0; i < pkt.tobject_size(); ++i)
	{
		auto tObject = pkt.mutable_tobject(i);

		_uint iObjectID = tObject->iobjectid();
		if (iObjectID == iUserPlayerID)
			continue;

		CGameObject* pObject = pGameInstance->Find_GameObejct(tObject->ilevel(), tObject->ilayer(), iObjectID);
		if (pObject == nullptr)
			continue;

		pObject->Set_TargetPos(Vec3(tObject->mutable_vtargetpos()->mutable_data()));
		pObject->Set_TargetMatrix(Matrix(tObject->mutable_matworld()->mutable_data()));
	}


	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_STATE_Client(PacketSessionRef& session, Protocol::S_STATE& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	auto tObject = pkt.tobject();

	CGameObject* pObject = pGameInstance->Find_GameObejct(tObject.ilevel(), tObject.ilayer(), tObject.iobjectid());
	if (pObject == nullptr || pObject->Is_Control() == true)
	{
		Safe_Release(pGameInstance);
		return true;
	}
		

	Vec3 vTargetPos(tObject.vtargetpos().data());
	Matrix matTargetWorld(tObject.matworld().data());


	pObject->Set_TargetPos(vTargetPos);
	pObject->Set_TargetMatrix(matTargetWorld);
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


	wstring strState = CAsUtils::ToWString(pkt.strstate());
	pObject->Set_NoneControlState(strState);

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_COLLIDERSTATE_Client(PacketSessionRef& session, Protocol::S_COLLIDERSTATE& pkt)
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

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_COLLISION_Client(PacketSessionRef& session, Protocol::S_COLLISION& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());
	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pObject->Hit_Collision(pkt.idamage(), Vec3(pkt.vhitpos().data()), pkt.istatuseffect(), pkt.fforce(), pkt.fduration());

	Safe_Release(pGameInstance);

	return true;
}


bool Handel_S_USERINFO_Client(PacketSessionRef& session, Protocol::S_USERINFO& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_NEARTARGET_Client(PacketSessionRef& session, Protocol::S_NEARTARGET& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	_int iTargetID = pkt.itargetobjectid();

	if (iTargetID == -1)
		pObject->Set_NearTarget(nullptr);
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.itargetobjectlayer(), iTargetID);
		if (pNearTarget == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		pObject->Set_NearTarget(pNearTarget);
	}

	//cout << CAsUtils::ToString(pObject->Get_ObjectTag()) << "  " << CAsUtils::ToString(pNearTarget->Get_ObjectTag()) << endl;

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_SLOWMOTION_Client(PacketSessionRef& session, Protocol::S_SLOWMOTION& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pObject->Set_SlowMotion(pkt.bslow());

	Safe_Release(pGameInstance);
	return true;
}


bool Handel_S_EVENT_Client(PacketSessionRef& session, Protocol::S_EVENT& pkt)
{
	if (pkt.istate() == (_uint)EVENTSTATE::READY)
	{
		CEventMgr::GetInstance()->Start_Event(pkt.ieventid());
	}
	else if (pkt.istate() == (_uint)EVENTSTATE::EVENT)
	{
		CEventMgr::GetInstance()->Get_Event(pkt.ieventid())->Set_State(pkt.istate());
	}
	else if (pkt.istate() == (_uint)EVENTSTATE::SETEND)
	{
		CEventMgr::GetInstance()->End_Event(pkt.ieventid());
	}

	return true;
}

bool Handel_S_HP_Client(PacketSessionRef& session, Protocol::S_HP& pkt)
{
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

bool Handel_S_CREATEPLAYER_Client(PacketSessionRef& session, Protocol::S_CREATE_PLAYER& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CPlayer::MODELDESC Desc;


	switch (pkt.iclass())
	{
	case (_uint)CHR_CLASS::GUNSLINGER:
		Desc.strFileName = L"Gunslinger";
		break;
	case (_uint)CHR_CLASS::SLAYER:
		Desc.strFileName = L"WR";
		break;
	}

	Matrix matWorld(pkt.matworld().data());

	Desc.bControl = pkt.bcontroll();
	Desc.iObjectID = pkt.iobjectid();
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_PLAYER;
	Desc.matWorld = matWorld;
	Desc.vTargetPos = Vec3(pkt.vtargetpos().data());
	Desc.szState = CAsUtils::ToWString(pkt.strstate());
	Desc.szNickName = CAsUtils::ToWString(pkt.strnickname());
	Desc.iWeaponIndex = pkt.iweaponindex();
	Desc.iCurrLevel = pkt.ilevel();

	wstring szProtoName = L"Prototype_GameObject_Player_" + Desc.strFileName;
	CPlayer* pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Add_GameObject(Desc.iCurrLevel, Desc.iLayer, szProtoName, &Desc));
	if (nullptr == pPlayer)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	if (Desc.bControl)
		CServerSessionManager::GetInstance()->Set_Player(pPlayer);


	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_CHAT_Client(PacketSessionRef& session, Protocol::S_CHAT& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CChat_Manager::GetInstance()->Add_Chat(CChat_Manager::S2W(pkt.szchat()));

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_MONSTERSTATE_Client(PacketSessionRef& session, Protocol::S_MONSTERSTATE& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pObject = pGameInstance->Find_GameObejct(pkt.ilevel(),(_uint) LAYER_TYPE::LAYER_MONSTER, pkt.iobjectid());


	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}
	_int iTargetID = pkt.itargetobjectid();




	if (iTargetID == -1)
		pObject->Set_NearTarget(nullptr);
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObejct(LEVEL_STATIC, pkt.itargetobjectlayer(), iTargetID);
		pObject->Set_NearTarget(pNearTarget);
	}
	
	
	
	dynamic_cast<CMonster*>(pObject)->Set_Action(CAsUtils::ToWString(pkt.strstate()));
	return true;
}
