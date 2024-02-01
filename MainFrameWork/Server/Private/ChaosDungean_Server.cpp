#include "stdafx.h"
#include "GameInstance.h"
#include "ChaosDungean_Server.h"
#include "Player_Server.h"
#include "Boss_Server.h"
#include "Monster_Server.h"
#include "GameSessionManager.h"
#include "AsUtils.h"
#include "NavigationMgr.h"
#include "GameSession.h"


CChaosDungean_Server::CChaosDungean_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"ChaosDungean", OBJ_TYPE::DUNGEAN)
{
}

CChaosDungean_Server::CChaosDungean_Server(const CChaosDungean_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CChaosDungean_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CChaosDungean_Server::Initialize(void* pArg)
{
	DUNGEANDESC* tDesc = static_cast<DUNGEANDESC*>(pArg);
	m_Players = tDesc->Players;
	Ready_ChaosDungean(tDesc->eLevel);

	m_fCurrSpawn = 0.0f;
	m_fSpawnDelay = 0.1f;
	
	Send_OpenLevel();

	m_bEnd = false;
	m_bStart = false;
	m_fEndDelay = 5.0f;

    return S_OK;
}

void CChaosDungean_Server::Tick(_float fTimeDelta)
{
	if (m_bStart == false)
	{
		Wait_For_Player();
		return;
	}

	if (m_fStartDelay > 0.0f)
	{
		m_fStartDelay -= fTimeDelta;
		if (m_fStartDelay < 0.0f)
			m_fStartDelay = 0.0f;

		return;
	}


	m_fCurrSpawn += fTimeDelta;
	if (m_fCurrSpawn > m_fSpawnDelay)
	{
		Spawn_Monster();
		m_fCurrSpawn = 0.0f;
	}

	if (m_iMonsterCount == 0 && m_iBossCount == 0 && m_Monsters.size() == 0 && m_Bosses.size() == 0)
	{
		m_bEnd = true;

		if (m_bEnd)
		{
			m_fEndDelay -= fTimeDelta;
			if (m_fEndDelay <= 0.0f)
			{
				if (m_eDungeanLevel == CHAOSDUNGEANLEVEL::LEVEL3)
					Exit_Dungean();
				else
					Enter_NextDungean();

				Set_Active(false);
				Set_Dead(true);
			}
		}
	}
}

void CChaosDungean_Server::LateTick(_float fTimeDelta)
{
	if (m_bStart == false)
	{
		return;
	}


	for (auto iterMonster = m_Monsters.begin(); iterMonster != m_Monsters.end();)
	{
		if ((*iterMonster)->Is_Dead())
			iterMonster = m_Monsters.erase(iterMonster);
		else
			++iterMonster;
	}

	for (auto iterBoss = m_Bosses.begin(); iterBoss != m_Bosses.end();)
	{
		if ((*iterBoss)->Is_Dead())
			iterBoss = m_Bosses.erase(iterBoss);
		else
			++iterBoss;
	}

	m_fBroadcastTime += fTimeDelta;
	if (m_fBroadcastTime >= 0.05f)
	{
		Broadcast_PlayerInfo();
		m_fBroadcastTime = 0.0f;
	}
}

HRESULT CChaosDungean_Server::Render()
{
    return S_OK;
}




HRESULT CChaosDungean_Server::Ready_Components()
{
    return S_OK;
}

HRESULT CChaosDungean_Server::Ready_ChaosDungean(CHAOSDUNGEANLEVEL eLevel)
{
	m_eDungeanLevel = eLevel;


	switch (m_eDungeanLevel)
	{
	case CHAOSDUNGEANLEVEL::LEVEL1:
		m_iCurrLevel = LEVELID::LEVEL_CHAOS_1;
		m_fStartDelay = 5.0f;
		m_iMonsterCount = 2;
		m_iMonsterMaxSpawnCount = 2;
		m_iBossCount = 0;
		m_MonsterSpawnList.push_back(L"Ghoul");
		m_MonsterSpawnList.push_back(L"Reaper");
		break;
	case CHAOSDUNGEANLEVEL::LEVEL2:
		m_iCurrLevel = LEVELID::LEVEL_CHAOS_2;
		m_fStartDelay = 5.0f;
		m_iMonsterCount = 30;
		m_iMonsterMaxSpawnCount = 30;
		m_iBossCount = 0;
		m_MonsterSpawnList.push_back(L"Zombie");
		m_MonsterSpawnList.push_back(L"Plant");	
		m_BossSpawnList.push_back(L"Golem");
		break;
	case CHAOSDUNGEANLEVEL::LEVEL3:
		m_iCurrLevel = LEVELID::LEVEL_CHAOS_3;
		m_fStartDelay = 5.0f;
		m_iMonsterCount = 50;
		m_iMonsterMaxSpawnCount = 10;
		m_iBossCount = 1;
		m_MonsterSpawnList.push_back(L"Pawn");
		m_BossSpawnList.push_back(L"King");
		break;
	}

	return S_OK;
}

void CChaosDungean_Server::Spawn_Monster()
{
	wstring szMonsterName;
	Vec3 vPos(0.0f, 0.0f, 0.0f);
	

	switch (m_eDungeanLevel)
	{
	case CHAOSDUNGEANLEVEL::LEVEL1:
	{
		vPos = Vec3(117.93f, 0.19f, 100.2f);
		_uint iMonster = rand() % 10;
		if (iMonster < 8)
			iMonster = 0;
		else
			iMonster = 1;
		szMonsterName = m_MonsterSpawnList[iMonster];
		vPos = Vec3(vPos.x + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f), vPos.y, vPos.z + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f));
		if (m_iBossCount > 0)
			Broadcast_Boss(vPos, m_BossSpawnList[0]);
	}
		break;
	case CHAOSDUNGEANLEVEL::LEVEL2:
	{
		vPos = Vec3(100.0f, 0.0f, 100.0f);
		_uint iMonster = CGameInstance::GetInstance()->Random_Int(0,9);
		if (iMonster < 8)
			iMonster = 0;
		else
			iMonster = 1;
		szMonsterName = m_MonsterSpawnList[iMonster];
		vPos = Vec3(vPos.x + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f), vPos.y, vPos.z + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f));

		if (m_iMonsterCount <= 3 && m_iBossCount >= 0 && m_Monsters.size() <= 3)
		{
			for (_uint i = 0; i < 3; ++i)
			{
				if (m_iBossCount > 0)
				{
					Broadcast_Boss(vPos, m_BossSpawnList[0]);
					vPos = Vec3(vPos.x + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f), vPos.y, vPos.z + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f));
				}
			}

			if (m_iBossCount > 0)
				m_iMonsterCount += 50;
		}
	}
		break;
	case CHAOSDUNGEANLEVEL::LEVEL3:
	{
		vPos = Vec3(99.84f, -1.8f, 111.9f);
		_uint iMonster = rand() % m_MonsterSpawnList.size();
		szMonsterName = m_MonsterSpawnList[iMonster];
		vPos = Vec3(vPos.x + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f), vPos.y, vPos.z + CGameInstance::GetInstance()->Get_RandomFloat(-4.f, 4.f));

		if (m_iBossCount > 0)
			Broadcast_Boss(vPos, m_BossSpawnList[0]);

	}
		break;
	}

	if(m_iMonsterCount > 0 && m_Monsters.size() < m_iMonsterMaxSpawnCount)
		Broadcast_Monster(vPos, szMonsterName);
}

void CChaosDungean_Server::Broadcast_Monster(Vec3 vPos, wstring ModelName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	{
		wstring szComponentName = L"Monster_" + ModelName;
		CMonster_Server::MODELDESC Desc;
		Desc.strFileName = ModelName;
		Desc.iObjectID = g_iObjectID++;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
		Desc.iLevel = m_iCurrLevel;

		wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
		CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(pGameInstance->Add_GameObject(m_iCurrLevel, Desc.iLayer, szMonsterName, &Desc));
		if (pMonster == nullptr)
			return;

		pMonster->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);
		CNavigationMgr::GetInstance()->Find_FirstCell(pMonster->Get_CurrLevel(), pMonster);
		m_Monsters.push_back(pMonster);
		--m_iMonsterCount;

		Protocol::S_CREATE_OBJCECT tMonsterPkt;

		tMonsterPkt.set_iobjectid(pMonster->Get_ObjectID());
		tMonsterPkt.set_iobjecttype(pMonster->Get_ObjectType());
		tMonsterPkt.set_strname(CAsUtils::ToString(pMonster->Get_ModelName()));
		tMonsterPkt.set_ilayer(pMonster->Get_ObjectLayer());
		tMonsterPkt.set_ilevel(m_iCurrLevel);

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

void CChaosDungean_Server::Broadcast_Boss(Vec3 vPos, wstring ModelName)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	{

		wstring szComponentName = L"Boss_" + ModelName;
		CBoss_Server::MODELDESC Desc;
		Desc.strFileName = ModelName;
		Desc.iObjectID = g_iObjectID++;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BOSS;
		Desc.iLevel = m_iCurrLevel;

		wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
		CBoss_Server* pBoss = dynamic_cast<CBoss_Server*>(pGameInstance->Add_GameObject(m_iCurrLevel, Desc.iLayer, szMonsterName, &Desc));
		if (pBoss == nullptr)
			return;

		pBoss->Get_TransformCom()->Set_State(CTransform::STATE::STATE_POSITION, vPos);
		CNavigationMgr::GetInstance()->Find_FirstCell(pBoss->Get_CurrLevel(), pBoss);
		m_Bosses.push_back(pBoss);
		--m_iBossCount;

		Protocol::S_CREATE_OBJCECT tMonsterPkt;

		tMonsterPkt.set_iobjectid(pBoss->Get_ObjectID());
		tMonsterPkt.set_iobjecttype(pBoss->Get_ObjectType());
		tMonsterPkt.set_strname(CAsUtils::ToString(pBoss->Get_ModelName()));
		tMonsterPkt.set_ilayer(pBoss->Get_ObjectLayer());
		tMonsterPkt.set_ilevel(pBoss->Get_CurrLevel());

		tMonsterPkt.set_bcontroll(true);

		auto vPos = tMonsterPkt.mutable_vpos();
		vPos->Resize(3, 0.0f);
		Vec3 vPosition = pBoss->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
		CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
		pBoss->Send_Monster_Action();
	}


	Safe_Release(pGameInstance);
}

void CChaosDungean_Server::Send_OpenLevel()
{
	Protocol::S_OPEN_LEVEL pkt;
	pkt.set_ilevelid(m_iCurrLevel);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	
	for (auto& Player : m_Players)
	{
		Player->Get_GameSession()->Set_LevelState(LEVELSTATE::INITREADY);
		Player->Get_GameSession()->Send(pSendBuffer);
	}
}

void CChaosDungean_Server::Enter_NextDungean()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);


	CChaosDungean_Server::DUNGEANDESC tDesc;
	tDesc.eLevel = CHAOSDUNGEANLEVEL((_uint)m_eDungeanLevel + 1);
	tDesc.Players = m_Players;

	pGameInstance->Add_GameObject(m_iCurrLevel + 1, (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Prototype_GameObject_ChaosDungean", &tDesc);


	Safe_Release(pGameInstance);
}

void CChaosDungean_Server::Exit_Dungean()
{
	Protocol::S_OPEN_LEVEL pkt;
	pkt.set_ilevelid(LEVEL_BERN);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);

	for (auto& Player : m_Players)
		Player->Get_GameSession()->Send(pSendBuffer);
}

HRESULT CChaosDungean_Server::Broadcast_PlayerInfo()
{
	Protocol::S_OBJECTINFO pkt;

	for (auto& Object : m_Players)
	{
		if (Object == nullptr)
			continue;

		auto tObject = pkt.add_tobject();
		tObject->set_iobjectid(Object->Get_ObjectID());
		tObject->set_ilevel(m_iCurrLevel);
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

	for (auto& Player : m_Players)
		dynamic_cast<CPlayer_Server*>(Player)->Get_GameSession()->Send(pSendBuffer);

	return S_OK;
}

void CChaosDungean_Server::Wait_For_Player()
{
	m_bStart = true;

	for (auto& Player : m_Players)
	{
		if (Player->Get_GameSession()->Get_LevelState() != LEVELSTATE::PLAYERREADY)
		{
			m_bStart = false;
		}
	}

	if (m_bStart == true)
	{
		Protocol::S_LEVEL_STATE pkt;
		pkt.set_ilevelstate(LEVELSTATE::INITEND);
		SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
		for (auto& Player : m_Players)
		{
			Player->Get_GameSession()->Send(pSendBuffer);
		}
	}
}

CChaosDungean_Server* CChaosDungean_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CChaosDungean_Server* pInstance = new CChaosDungean_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CChaosDungean_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CChaosDungean_Server::Clone(void* pArg)
{
	CChaosDungean_Server* pInstance = new CChaosDungean_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CChaosDungean_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CChaosDungean_Server::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	for (auto& Monster : m_Monsters)
		Monster->Set_Dead(true);

	for (auto& Boss : m_Bosses)
		Boss->Set_Dead(true);
}
