#include "stdafx.h"
#include "GameInstance.h"
#include "ValtanMain_Server.h"
#include "Player_Server.h"
#include "Boss_Server.h"
#include "Monster_Server.h"
#include "GameSessionManager.h"
#include "AsUtils.h"
#include "NavigationMgr.h"
#include "GameSession.h"


CValtanMain_Server::CValtanMain_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"VALTANMAIN", OBJ_TYPE::DUNGEAN)
{
}

CValtanMain_Server::CValtanMain_Server(const CValtanMain_Server& rhs)
	: CGameObject(rhs)
{
}

HRESULT CValtanMain_Server::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Initialize(void* pArg)
{
	DUNGEANDESC* tDesc = static_cast<DUNGEANDESC*>(pArg);
	m_Players = tDesc->Players;
	Ready_Dungean();

	
	Send_OpenLevel();

	m_bEnd = false;
	m_fEndDelay = 5.0f;


    return S_OK;
}

void CValtanMain_Server::Tick(_float fTimeDelta)
{
	if (m_fStartDelay > 0.0f)
	{
		m_fStartDelay -= fTimeDelta;
		if (m_fStartDelay < 0.0f)
		{
			m_fStartDelay = 0.0f;
			Broadcast_Boss(Vec3(117.93f, 0.19f, 100.2f), L"Valtan");
		}
			
		return;
	}

	if (m_iBossCount == 0 && m_Bosses.size() == 0)
	{
		/*m_bEnd = true;

		if (m_bEnd)
		{
			m_fEndDelay -= fTimeDelta;
			if (m_fEndDelay <= 0.0f)
			{

				Set_Active(false);
				Set_Dead(true);
			}
		}*/
	}
}

void CValtanMain_Server::LateTick(_float fTimeDelta)
{
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

HRESULT CValtanMain_Server::Render()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Ready_Components()
{
    return S_OK;
}

HRESULT CValtanMain_Server::Ready_Dungean()
{
	m_iCurrLevel = LEVELID::LEVEL_VALTANMAIN;
	m_fStartDelay = 5.0f;
	m_iBossCount = 0;
	m_iBossCount = 1;
	return S_OK;
}


void CValtanMain_Server::Broadcast_Boss(Vec3 vPos, wstring ModelName)
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

void CValtanMain_Server::Send_OpenLevel()
{
	Protocol::S_OPEN_LEVEL pkt;
	pkt.set_ilevelid(m_iCurrLevel);

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(pkt);
	
	for (auto& Player : m_Players)
		Player->Get_GameSession()->Send(pSendBuffer);
}

void CValtanMain_Server::Exit_Dungean()
{
	
}

HRESULT CValtanMain_Server::Broadcast_PlayerInfo()
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

CValtanMain_Server* CValtanMain_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CValtanMain_Server* pInstance = new CValtanMain_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CValtanMain_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CValtanMain_Server::Clone(void* pArg)
{
	CValtanMain_Server* pInstance = new CValtanMain_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CValtanMain_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtanMain_Server::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);

	for (auto& Boss : m_Bosses)
		Boss->Set_Dead(true);
}
