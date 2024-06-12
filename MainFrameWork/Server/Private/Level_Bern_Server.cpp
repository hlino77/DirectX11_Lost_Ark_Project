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
#include "Party_Server.h"

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
	m_bKey_Lock = false;

	return S_OK;
}

HRESULT CLevel_Bern_Server::Tick(_float fTimeDelta)
{

	if (m_bKey_Lock == false)
	{
		if (GetAsyncKeyState('V') & 0x8000 && GetAsyncKeyState('1') & 0x8000)
		{
			if (m_bKey_Lock)
				return S_OK;
			m_bKey_Lock = true;

			Enter_ValtanMain();
		}
	}
	


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
	pGameInstance->Add_GameObject(LEVEL_BERN, (_uint)eLayerType, TEXT("Prototype_GameObject_Guide_Valtan_Npc"));

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

	vector<CPlayer_Server*> Players = dynamic_cast<CPlayer_Server*>(LevelObjects.front())->Get_Party()->Get_Players();

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
