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
#include "Party.h"
#include "Skill_RisingSun.h"
#include "Skill_TeleportDoor.h"
#include "Item_Manager.h"
#include "Item.h"

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
			break;
		}
	}

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_LEVELSTATE_Client(PacketSessionRef& session, Protocol::S_LEVEL_STATE& pkt)
{
	CServerSessionManager::GetInstance()->Get_ServerSession()->Set_LevelState((LEVELSTATE)pkt.ilevelstate());

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
		Desc.bInstance = true;
		Desc.iLevel = pkt.ilevel();

		wstring szProtoName = L"Prototype_GameObject_" + Desc.strFileName;
		CMonster* pMonster = dynamic_cast<CMonster*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szProtoName, &Desc));
		if (nullptr == pMonster)
		{
			Safe_Release(pGameInstance);
			return true;
		}
		break;
	}
	case OBJ_TYPE::BOSS:
	{
		CBoss::MODELDESC Desc;
		Desc.strFileName = L"Boss_" + CAsUtils::ToWString(pkt.strname());
		Desc.iObjectID = pkt.iobjectid();
		Desc.iLayer = pkt.ilayer();
		Desc.iLevel = pkt.ilevel();
		Desc.vPos = Vec3(pkt.vpos().data());

		wstring szProtoName = L"Prototype_GameObject_" + Desc.strFileName;
		CBoss* pBoss = dynamic_cast<CBoss*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szProtoName, &Desc));
		if (nullptr == pBoss)
		{
			Safe_Release(pGameInstance);
			return true;
		}
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

	return true;
}


bool Handel_S_ANIMATION_Client(PacketSessionRef& session, Protocol::S_ANIMATION& pkt)
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

bool Handel_S_OBJECTINFO_Client(PacketSessionRef& session, Protocol::S_OBJECTINFO& pkt)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (pPlayer == nullptr)
		return true;
	_uint iUserPlayerID = pPlayer->Get_ObjectID();


	for (_uint i = 0; i < (_int)pkt.tobject_size(); ++i)
	{
		auto tObject = pkt.mutable_tobject(i);

		_uint iObjectID = tObject->iobjectid();
		if (iObjectID == iUserPlayerID)
			continue;

		CGameObject* pObject = pGameInstance->Find_GameObject(tObject->ilevel(), tObject->ilayer(), iObjectID);
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

	CGameObject* pObject = pGameInstance->Find_GameObject(tObject.ilevel(), tObject.ilayer(), tObject.iobjectid());
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

	_uint iTargetID = pkt.itargetobjectid();

	if (iTargetID == -1)
		pObject->Reset_NearTarget();
	else if (iTargetID == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID())
		pObject->Set_NearTarget(CServerSessionManager::GetInstance()->Get_Player());
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObject(tObject.ilevel(), pkt.itargetobjectlayer(), iTargetID);
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

	wstring strState = CAsUtils::ToWString(pkt.strstate());
	pObject->Set_NoneControlState(strState);

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_COLLIDERSTATE_Client(PacketSessionRef& session, Protocol::S_COLLIDERSTATE& pkt)
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

	Safe_Release(pGameInstance);

	return true;
}

bool Handel_S_COLLISION_Client(PacketSessionRef& session, Protocol::S_COLLISION& pkt)
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


	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	_int iTargetID = pkt.itargetobjectid();

	if (iTargetID == -1)
		pObject->Set_NearTarget(nullptr);
	else if (iTargetID == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID())
		pObject->Set_NearTarget(CServerSessionManager::GetInstance()->Get_Player());
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.itargetobjectlayer(), iTargetID);
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

	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

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


	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

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

	Matrix matWorld(pkt.matworld().data());

	Desc.bControl = pkt.bcontroll();
	Desc.iObjectID = pkt.iobjectid();
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_PLAYER;
	Desc.matWorld = matWorld;
	Desc.vTargetPos = Vec3(pkt.vtargetpos().data());
	Desc.szState = CAsUtils::ToWString(pkt.strstate());
	Desc.szNickName = CAsUtils::S2W(pkt.strnickname());
	Desc.iWeaponIndex = pkt.iweaponindex();
	Desc.iCurrLevel = pkt.ilevel();
	Desc.pItemCodes = (_int*)pkt.itemcodes().data();

	wstring szProtoName = L"Prototype_GameObject_Player_" + Desc.strFileName;

	CPlayer* pPlayer = nullptr;
	if(Desc.bControl)
		pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Add_GameObject(LEVELID::LEVEL_STATIC, Desc.iLayer, szProtoName, &Desc));
	else
		pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Add_GameObject(Desc.iCurrLevel, Desc.iLayer, szProtoName, &Desc));
		
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


	CChat_Manager::GetInstance()->Add_Chat(pkt.iobjectid(), pkt.ilevel() ,CAsUtils::S2W(pkt.szchat()));

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_MONSTERSTATE_Client(PacketSessionRef& session, Protocol::S_MONSTERSTATE& pkt)
{

	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer() , pkt.iobjectid());


	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}
	_int iTargetID = pkt.itargetobjectid();




	if (iTargetID == -1)
		pObject->Set_NearTarget(nullptr);
	else if (iTargetID == CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID())
		pObject->Set_NearTarget(CServerSessionManager::GetInstance()->Get_Player());
	else
	{
		CGameObject* pNearTarget = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.itargetobjectlayer(), iTargetID);
		pObject->Set_NearTarget(pNearTarget);
	}

	dynamic_cast<CMonster*>(pObject)->Set_RandomPosition(Vec3(pkt.vtargetpos().data()));

	dynamic_cast<CMonster*>(pObject)->Set_Action(CAsUtils::ToWString(pkt.strstate()));
	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_PLAYERLEVELMOVE_Client(PacketSessionRef& session, Protocol::S_PLAYERLEVELMOVE& pkt)
{

	return true;
}

bool Handel_S_DELETEGAMEOBJECT_Client(PacketSessionRef& session, Protocol::S_DELETEGAMEOBJECT& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
	CGameObject* pObject = pGameInstance->Find_GameObject(pkt.ilevel(), pkt.ilayer(), pkt.iobjectid());

	if (pObject == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pObject->Set_Dead(true);

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_IDENTITY_Client(PacketSessionRef& session, Protocol::S_IDENTITY& pkt)
{
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

bool Handel_S_PARTY_Client(PacketSessionRef& session, Protocol::S_PARTY& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CPlayer* pPlayer = CServerSessionManager::GetInstance()->Get_Player();

	if (pkt.tcreateparty().empty() == false)
	{
		if (pPlayer->Get_Party() != nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		auto& tCreateParty = pkt.tcreateparty(0);

		vector<CGameObject*> Players;
		

		for (_uint i = 0; i < (_uint)tCreateParty.tplayers().size(); ++i)
		{
			_uint iObjectID = tCreateParty.tplayers(i).iid();

			if (iObjectID == pPlayer->Get_ObjectID())
			{
				Players.push_back(pPlayer);
				continue;
			}

			CGameObject* pObject = pGameInstance->Find_GameObject(tCreateParty.tplayers(i).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iObjectID);

			if (pObject == nullptr)
			{
				Safe_Release(pGameInstance);
				return true;
			}
			Players.push_back(pObject);

		}


		pPlayer->Set_Party(new CParty(Players));
	}
	else if (pkt.tjoinparty().empty() == false)
	{
		auto& tJoinParty = pkt.tjoinparty(0);

		CGameObject* pObject = pGameInstance->Find_GameObject(tJoinParty.tplayer(0).ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, tJoinParty.tplayer(0).iid());
		if (pObject == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		pPlayer->Get_Party()->Add_Player(pObject);
	}
	else if (pkt.tinvitationparty().empty() == false)
	{
		auto& tInvitation = pkt.tinvitationparty(0);

		
	}

	Safe_Release(pGameInstance);
	return true;
}


bool Handel_S_NPC_Client(PacketSessionRef& session, Protocol::S_NPC& pkt)
{


	return true;
}

bool Handel_S_NAVIGATION_Client(PacketSessionRef& session, Protocol::S_NAVIGATION& pkt)
{

	return true;
}

bool Handel_S_CREATESKILL_Client(PacketSessionRef& session, Protocol::S_CREATE_SKILL& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	switch ((SKILL_TYPE)pkt.iskilltype())
	{
	case SKILL_TYPE::SKILL_RISINGSUN:
	{
		_uint iPlayerID = pkt.iplayerid();
		CGameObject* pPlayer = nullptr;

		if (CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID() == iPlayerID)
		{
			pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		}
		else
		{
			pPlayer = pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID);
		}

		if (pPlayer == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		CSkill_RisingSun::RisingSunDesc tDesc;
		tDesc.iObjectID = pkt.iskillid();
		tDesc.pSkillOwner = pPlayer;

		Matrix matWorld(pkt.matrix().data());
		tDesc.matWorld = &matWorld;

		CGameObject* pObject = pGameInstance->Add_GameObject(pPlayer->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_RisingSun", &tDesc);

	}
	
		break;

	case SKILL_TYPE::SKILL_TELEPORTDOOR:
	{
		_uint iPlayerID = pkt.iplayerid();
		CGameObject* pPlayer = nullptr;

		if (CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID() == iPlayerID)
		{
			pPlayer = CServerSessionManager::GetInstance()->Get_Player();
		}
		else
		{
			pPlayer = pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID);
		}

		if (pPlayer == nullptr)
		{
			Safe_Release(pGameInstance);
			return true;
		}

		CSkill_TeleportDoor::RisingSunDesc tDesc;
		tDesc.iObjectID = pkt.iskillid();
		tDesc.pSkillOwner = pPlayer;

		Matrix matWorld(pkt.matrix().data());
		tDesc.matWorld = &matWorld;

		CGameObject* pObject = pGameInstance->Add_GameObject(pPlayer->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_TeleportDoor", &tDesc);

	}
		break;
	}

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_BUFFSKILL_Client(PacketSessionRef& session, Protocol::S_BUFF_SKILL& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CSkill* pSkill = dynamic_cast<CSkill*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_SKILL, pkt.iskillid()));
	
	if (pSkill == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pSkill->Buff_Player(pkt.iplayerid());

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_PLAYERTELEPORT_Client(PacketSessionRef& session, Protocol::S_PLAYERTELEPORT& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iPlayerID = pkt.iplayerid();
	CPlayer* pPlayer = nullptr;

	if (CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID() == iPlayerID)
	{
		pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	}
	else
	{
		pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));
	}

	if (pPlayer == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	pPlayer->Set_Render(false);
	pPlayer->Set_NoneControlState(L"Idle");

	Matrix matWorld(pkt.matrix().data());
	pPlayer->Set_TargetMatrix(matWorld);
	pPlayer->Get_TransformCom()->Set_WorldMatrix(matWorld);
	pPlayer->Set_Render(true);

	Safe_Release(pGameInstance);
	return true;
}

bool Handel_S_CHANGEEQUIP_Client(PacketSessionRef& session, Protocol::S_CHANGEEQUIP& pkt)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	_uint iPlayerID = pkt.iplayerid();
	CPlayer* pPlayer = nullptr;

	if (CServerSessionManager::GetInstance()->Get_Player()->Get_ObjectID() == iPlayerID)
	{
		pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	}
	else
	{
		pPlayer = dynamic_cast<CPlayer*>(pGameInstance->Find_GameObject(pkt.ilevel(), (_uint)LAYER_TYPE::LAYER_PLAYER, iPlayerID));
	}

	if (pPlayer == nullptr)
	{
		Safe_Release(pGameInstance);
		return true;
	}

	for (_uint i = 0; i < (_uint)CItem::PART::_END; ++i)
	{
		_int iItemCode = pkt.itemcodes(i);

		if (iItemCode == -1)
		{
			if (pPlayer->Get_EquipItem(i) != nullptr)
			{
				pPlayer->Get_EquipItem(i)->Disuse_Item(pPlayer, true);
			}

			continue;
		}
		else if (pPlayer->Get_EquipItem(i) == nullptr || (_int)pPlayer->Get_EquipItem(i)->Get_ItemCode() != iItemCode)
		{
			CItem* pItem = CItem_Manager::GetInstance()->Get_Item((ITEMCODE)iItemCode);

			if (pItem == nullptr)
			{
				Safe_Release(pGameInstance);
				return true;
			}

			pItem->Use_Item(pPlayer);
		}
	}

	return true;
}

