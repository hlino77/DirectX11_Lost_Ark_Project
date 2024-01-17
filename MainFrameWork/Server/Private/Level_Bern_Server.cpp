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
#include "Monster_Zombie_Server.h"
#include "ChaosDungean_Server.h"
#include "Guide_Chaos_Npc_Server.h"
#include "ValtanMain_Server.h"

CLevel_Bern_Server::CLevel_Bern_Server()
	: CLevel(nullptr, nullptr)
{
}

HRESULT CLevel_Bern_Server::Initialize()
{
	//CNavigationMgr::GetInstance()->Set_CurrNavigation(TEXT("Level_Choas1_Navi"));


	Ready_Events();

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

	if (FAILED(Ready_Layer_Npc(LAYER_TYPE::LAYER_NPC)))
		return E_FAIL;


	m_fNextLevelDelay = 3.0f;

	return S_OK;
}

HRESULT CLevel_Bern_Server::Tick(const _float& fTimeDelta)
{
	if (GetAsyncKeyState('V') & 0x8000 && GetAsyncKeyState('1') & 0x8000)
	{
		if (m_bKey_Lock)
			return S_OK;
		m_bKey_Lock = true;

		Enter_ValtanMain();
	}
	else
		m_bKey_Lock = false;


	return S_OK;
}

HRESULT CLevel_Bern_Server::LateTick(const _float& fTimeDelta)
{
	m_fBroadcastTime += fTimeDelta;
	if (m_fBroadcastTime >= 0.05f)
	{
		Broadcast_PlayerInfo();
		m_fBroadcastTime = 0.0f;
		auto& ObjectList = CGameInstance::GetInstance()->Find_GameObjects(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER);
		if (ObjectList.size() == 0)
			return S_OK;
		else
		{
			Vec3 vPos = CGameInstance::GetInstance()->Find_GameObjects(LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER).front()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 MonsterPos = Vec3(vPos.x + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f), vPos.y, vPos.z + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f));

			if (GetAsyncKeyState(VK_CONTROL) & 0x8000&& GetAsyncKeyState('1') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Monster(MonsterPos, L"Zombie");

			}
			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('2') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Boss(L"Golem", MonsterPos);

			}
			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('3') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Monster(MonsterPos, L"Ghoul");

			}
			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('4') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Monster(MonsterPos, L"Plant");
			}
			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('5') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Monster(MonsterPos, L"Reaper");
			}
			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('6') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Boss( L"King", MonsterPos);
			}

			else if (GetAsyncKeyState(VK_CONTROL) & 0x8000 && GetAsyncKeyState('7') & 0x8000)
			{
				if (m_bKey_Lock)
					return S_OK;
				m_bKey_Lock = true;
				Broadcast_Monster( MonsterPos, L"Pawn");
			}
			else
				m_bKey_Lock = false;

		}
	}
	return S_OK;
}

HRESULT CLevel_Bern_Server::Exit()
{
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

HRESULT CLevel_Bern_Server::Ready_Layer_Npc(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	pGameInstance->Add_GameObject(LEVEL_BERN, (_uint)eLayerType, TEXT("Prototype_GameObject_Guide_Chaos_Npc"));

	RELEASE_INSTANCE(CGameInstance);

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
	}


	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	
	for (auto& Player : ObjectList)
		dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pSendBuffer);

	return S_OK;
}

HRESULT CLevel_Bern_Server::Broadcast_Monster(Vec3 vPos, wstring ModelName)
{


	{

		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		{
			ModelName = L"Monster_" + ModelName;
			CMonster_Server::MODELDESC Desc;
			Desc.strFileName = ModelName;
			Desc.iObjectID = g_iObjectID++;
			Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
			Desc.iLevel = LEVEL_BERN;

			wstring szMonsterName = L"Prototype_GameObject_" + ModelName;
			CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(pGameInstance->Add_GameObject(LEVEL_BERN, Desc.iLayer, szMonsterName, &Desc));
			if (pMonster == nullptr)
				return E_FAIL;

			pMonster->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);

			CNavigationMgr::GetInstance()->Find_FirstCell(pMonster->Get_CurrLevel(), pMonster);
			Protocol::S_CREATE_OBJCECT tMonsterPkt;

			tMonsterPkt.set_iobjectid(pMonster->Get_ObjectID());
			tMonsterPkt.set_iobjecttype(pMonster->Get_ObjectType());
			tMonsterPkt.set_strname(CAsUtils::ToString(pMonster->Get_ModelName()));
			tMonsterPkt.set_ilayer(pMonster->Get_ObjectLayer());
			tMonsterPkt.set_ilevel(LEVELID::LEVEL_BERN);

			tMonsterPkt.set_bcontroll(true);

			auto vPos = tMonsterPkt.mutable_vpos();
			vPos->Resize(3, 0.0f);
			Vec3 vPosition = pMonster->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

			SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
			CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
			static_cast<CMonster_Server*>(pMonster)->Send_Monster_Action();
		}
		Safe_Release(pGameInstance);
	}


	return S_OK;
}

HRESULT CLevel_Bern_Server::Broadcast_Boss(const wstring& szName, Vec3 vPos)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	{

		wstring ModelName = L"Boss_" + szName;
		CBoss_Server::MODELDESC Desc;
		Desc.strFileName = ModelName;
		Desc.iObjectID = g_iObjectID++;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;

		wstring szMonsterName = L"Prototype_GameObject_" + ModelName;
		CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(pGameInstance->Add_GameObject(LEVEL_BERN, Desc.iLayer, szMonsterName, &Desc));
		if (pMonster == nullptr)
			return E_FAIL;

		pMonster->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);

		CNavigationMgr::GetInstance()->Find_FirstCell(pMonster->Get_CurrLevel(), pMonster);
		
		Protocol::S_CREATE_OBJCECT tMonsterPkt;

		tMonsterPkt.set_iobjectid(pMonster->Get_ObjectID());
		tMonsterPkt.set_iobjecttype(pMonster->Get_ObjectType());
		tMonsterPkt.set_strname(CAsUtils::ToString(pMonster->Get_ModelName()));
		tMonsterPkt.set_ilayer(pMonster->Get_ObjectLayer());
		tMonsterPkt.set_ilevel(LEVELID::LEVEL_BERN);

		tMonsterPkt.set_bcontroll(true);

		auto vPos = tMonsterPkt.mutable_vpos();
		vPos->Resize(3, 0.0f);
		Vec3 vPosition = pMonster->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
		CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
		static_cast<CMonster_Server*>(pMonster)->Send_Monster_Action();
	}


	Safe_Release(pGameInstance);




	return S_OK;
}

HRESULT CLevel_Bern_Server::Ready_Events()
{

	return S_OK;
}



void CLevel_Bern_Server::Enter_ChaosDungean(CHAOSDUNGEANLEVEL eLEVEL)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	vector<CGameObject*> LevelObjects = pGameInstance->Find_GameObjects(LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_PLAYER);

	if (LevelObjects.empty())
		return;

	vector<CPlayer_Server*> Players;

	for (auto& Object : LevelObjects)
	{
		CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(Object);
		if (pPlayer)
			Players.push_back(pPlayer);
	}

	CChaosDungean_Server::DUNGEANDESC tDesc;
	tDesc.eLevel = eLEVEL;
	tDesc.Players = Players;

	pGameInstance->Add_GameObject(LEVEL_CHAOS_1, (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Prototype_GameObject_ChaosDungean", &tDesc);


	Safe_Release(pGameInstance);
}

void CLevel_Bern_Server::Enter_ValtanMain()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	vector<CGameObject*> LevelObjects = pGameInstance->Find_GameObjects(LEVEL_BERN, (_uint)LAYER_TYPE::LAYER_PLAYER);

	if (LevelObjects.empty())
		return;

	vector<CPlayer_Server*> Players;

	for (auto& Object : LevelObjects)
	{
		CPlayer_Server* pPlayer = dynamic_cast<CPlayer_Server*>(Object);
		if (pPlayer)
			Players.push_back(pPlayer);
	}

	CValtanMain_Server::DUNGEANDESC tDesc;
	tDesc.Players = Players;
	pGameInstance->Add_GameObject(LEVEL_VALTANMAIN, (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Prototype_GameObject_ValtanMain", &tDesc);

	Safe_Release(pGameInstance);
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
