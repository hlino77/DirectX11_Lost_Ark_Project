#include "stdafx.h"
#include "Level_Bern_Server.h"
#include "GameSessionManager.h"
#include "GameSession.h"
#include "AsUtils.h"
#include "Player_Server.h"
#include "GameInstance.h"
#include "CollisionManager.h"
#include "ThreadManager.h"
#include "Struct.pb.h"
#include "NavigationMgr.h"
#include "Boss_Server.h"
#include "EventMgr.h"
#include "Level_Loading_Server.h"
#include "Monster_Server.h"



CLevel_Bern_Server::CLevel_Bern_Server()
	: CLevel(nullptr, nullptr)
{
}

HRESULT CLevel_Bern_Server::Initialize()
{
	CNavigationMgr::GetInstance()->Add_Navigation(L"Arena.navi");

	Ready_Events();


	Broadcast_LevelState(LEVELSTATE::INITREADY);
	Wait_ClientLevelState(LEVELSTATE::INITREADY);
	Broadcast_LevelState(LEVELSTATE::INITSTART);


	if (FAILED(__super::Initialize()))
		return E_FAIL;

	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(LAYER_TYPE::LAYER_PLAYER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Monster(LAYER_TYPE::LAYER_MONSTER)))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(LAYER_TYPE::LAYER_EFFECT)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	Wait_ClientLevelState(LEVELSTATE::INITEND);

	CEventMgr::GetInstance()->Start_Event((_uint)EVENT::ARENASTART);

	Broadcast_LevelState(LEVELSTATE::INITEND);


	Start_Collision();

	m_fNextLevelDelay = 3.0f;

	return S_OK;
}

HRESULT CLevel_Bern_Server::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::LateTick(_float fTimeDelta)
{
	m_fBroadcastTime += fTimeDelta;
	if (m_fBroadcastTime >= 0.05f)
	{
		Broadcast_PlayerInfo();
		m_fBroadcastTime = 0.0f;
	}

	return S_OK;
}

HRESULT CLevel_Bern_Server::Exit()
{
	CNavigationMgr::GetInstance()->Reset_Navigation();
	End_Collision();
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Lights()
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_Camera(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_Monster(const LAYER_TYPE eLayerType)
{
	/*Vec3 vPos(-20.0f, 0.0f, 5.0f);

	for (_uint i = 0; i < 5; ++i)
	{
		if (FAILED(Broadcast_Monster(L"WhiteZetsu", vPos)))
			return E_FAIL;


		vPos.x += 0.01f;
	}*/
	
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_UI(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Layer_Effect(const LAYER_TYPE eLayerType)
{
	return S_OK;
}

HRESULT CLevel_Bern_Server::Broadcast_Character()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_Bern_Server::Broadcast_LevelState(LEVELSTATE eState)
{
	Protocol::S_LEVEL_STATE pkt;
	pkt.set_ilevelstate((uint32)eState);

	SendBufferRef sendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(sendBuffer);
}

void CLevel_Bern_Server::Wait_ClientLevelState(LEVELSTATE eState)
{
	set<GameSessionRef>& Sessions = CGameSessionManager::GetInstance()->Get_Sessions();

	while (true)
	{
		_uint iReadyCount = 0;

		for (auto& Session : Sessions)
		{
			if (Session->Get_LevelState() == eState)
				++iReadyCount;
		}

		if (iReadyCount == CGameSessionManager::GetInstance()->Get_SessionCount())
			break;
	}
}

HRESULT CLevel_Bern_Server::Broadcast_PlayerInfo()
{
	auto& ObjectList = CGameInstance::GetInstance()->Find_GameObjects(LEVELID::LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_PLAYER);

	if (ObjectList.size() == 0)
		return S_OK;

	Protocol::S_OBJECTINFO pkt;

	for (auto& Object : ObjectList)
	{
		if (Object == nullptr)
			continue;

		auto tObject = pkt.add_tobject();
		tObject->set_iobjectid(Object->Get_ObjectID());
		tObject->set_ilevel(LEVELID::LEVEL_BERN);
		tObject->set_ilayer((_uint)LAYER_TYPE::LAYER_PLAYER);


		auto vTargetPos = tObject->mutable_vtargetpos();
		vTargetPos->Resize(3, 0.0f);
		Vec3 vPlayerTargetPos = Object->Get_TargetPos();
		memcpy(vTargetPos->mutable_data(), &vPlayerTargetPos, sizeof(Vec3));


		auto matWorld = tObject->mutable_matworld();
		matWorld->Resize(16, 0.0f);
		Matrix matPlayerWorld = Object->Get_TransformCom()->Get_WorldMatrix();
		memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

		/*if (pPlayer->Get_ObjectTag() == L"Naruto")
			cout << matPlayerWorld.m[3][0] << " " << matPlayerWorld.m[3][1] << " " << matPlayerWorld.m[3][2] << endl;*/
	}


	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);


	return S_OK;
}

HRESULT CLevel_Bern_Server::Broadcast_Monster(const wstring& szName, Vec3 vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	Protocol::S_CREATE_OBJCECT pkt;
	pkt.set_strname(CAsUtils::ToString(szName));
	pkt.set_iobjectid(g_iObjectID++);
	pkt.set_ilevel((uint32)LEVELID::LEVEL_BERN);
	pkt.set_ilayer((uint32)LAYER_TYPE::LAYER_MONSTER);
	pkt.set_iobjecttype((uint32)OBJ_TYPE::MONSTER);

	auto vPacketPos = pkt.mutable_vpos();
	vPacketPos->Resize(3, 0.0f);
	memcpy(vPacketPos->mutable_data(), &vPos, sizeof(Vec3));

	/*auto tMonster = pkt.add_tmonsterinfo();
	tMonster->set_ffollowdistance(10.0f);*/

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);


	CMonster_Server::MODELDESC Desc;
	Desc.strFileName = CAsUtils::ToWString(pkt.strname());
	Desc.iObjectID = pkt.iobjectid();
	Desc.iLayer = pkt.ilayer();

	wstring szMonsterName = L"Prototype_GameObject_Monster_" + szName;
	CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szMonsterName, &Desc));
	if (pMonster == nullptr)
		return E_FAIL;

	pMonster->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);

	CNavigationMgr::GetInstance()->Find_FirstCell(pMonster);


	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CLevel_Bern_Server::Broadcast_Boss(const wstring& szName, Vec3 vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	{
		Protocol::S_CREATE_OBJCECT pkt;
		pkt.set_strname(CAsUtils::ToString(szName));
		pkt.set_iobjectid(g_iObjectID++);
		pkt.set_ilevel((uint32)LEVELID::LEVEL_BERN);
		pkt.set_ilayer((uint32)LAYER_TYPE::LAYER_BOSS);
		pkt.set_iobjecttype((uint32)OBJ_TYPE::BOSS);

		auto vPacketPos = pkt.mutable_vpos();
		vPacketPos->Resize(3, 0.0f);
		memcpy(vPacketPos->mutable_data(), &vPos, sizeof(Vec3));

		/*auto tMonster = pkt.add_tmonsterinfo();
		tMonster->set_ffollowdistance(10.0f);*/

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
		CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);


		CBoss_Server::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(pkt.strname());
		Desc.iObjectID = pkt.iobjectid();
		Desc.iLayer = pkt.ilayer();

		wstring szMonsterName = L"Prototype_GameObject_Boss_" + szName;
		CBoss_Server* pBoss = dynamic_cast<CBoss_Server*>(pGameInstance->Add_GameObject(pkt.ilevel(), pkt.ilayer(), szMonsterName, &Desc));
		if (pBoss == nullptr)
			return E_FAIL;

		pBoss->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);
		pBoss->Set_Skill(nullptr);
		CNavigationMgr::GetInstance()->Find_FirstCell(pBoss);
	}


	Safe_Release(pGameInstance);




	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Events()
{

	return S_OK;
}



void CLevel_Bern_Server::Set_CheckGruop()
{
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY, (_uint)LAYER_COLLIDER::LAYER_BODY);
	CCollisionManager::GetInstance()->CheckGroup((_uint)LAYER_COLLIDER::LAYER_BODY, (_uint)LAYER_COLLIDER::LAYER_ATTACK);
}

void CLevel_Bern_Server::Start_Collision()
{
	Set_CheckGruop();

	m_pCollisionThread = new thread([=]()
		{
			ThreadManager::GetInstance()->InitTLS();

			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CCollisionManager* pCollisionManager = CCollisionManager::GetInstance();
			pCollisionManager->AddRef();

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision_Bern"))))
				return FALSE;

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_Collision_60_Bern"))))
				return FALSE;

			_float		fTimeAcc = 0.f;


			while (!pCollisionManager->Is_Stop())
			{
				fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision_Bern"));

				if (fTimeAcc >= 1.f / 60.0f)
				{
					pCollisionManager->LateTick_Collision(pGameInstance->Compute_TimeDelta(TEXT("Timer_Collision_60_Bern")));
					fTimeAcc = 0.f;
				}
			}

			Safe_Release(pCollisionManager);

			Safe_Release(pGameInstance);

			ThreadManager::GetInstance()->DestroyTLS();
		});
}

void CLevel_Bern_Server::End_Collision()
{
	CCollisionManager::GetInstance()->Set_Stop(true);
	m_pCollisionThread->join();
	CCollisionManager::GetInstance()->Reset();
	Safe_Delete(m_pCollisionThread);
}


CLevel_Bern_Server* CLevel_Bern_Server::Create()
{
	CLevel_Bern_Server* pInstance = new CLevel_Bern_Server();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Bern_Server::Free()
{
	__super::Free();
}
