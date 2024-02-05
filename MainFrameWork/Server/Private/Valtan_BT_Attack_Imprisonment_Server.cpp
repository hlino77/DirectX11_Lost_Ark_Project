#include "stdafx.h"
#include "Valtan_BT_Attack_Imprisonment_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
#include <Boss_Valtan_Server.h>
#include "GameInstance.h"
#include <AsUtils.h>
#include "GameSessionManager.h"
#include "Player_Server.h"
#include <Monster_Prison_Server.h>
CValtan_BT_Attack_Imprisonment_Server::CValtan_BT_Attack_Imprisonment_Server()
{
}

void CValtan_BT_Attack_Imprisonment_Server::OnStart()
{
	__super::OnStart(0);
	vector<CGameObject*> vecTargets = CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_PLAYER);

	auto iter = vecTargets.begin();
	for (auto& pPlayer : vecTargets)
	{
		if (TEXT("Dead_End") == static_cast<CPlayer_Server*>(pPlayer)->Get_ServerState())
		{
			iter = vecTargets.erase(iter);
		}
		else
			iter++;
	}
	if (vecTargets.size() > 0)
	{
		CGameObject* pRandomTarget = vecTargets[CGameInstance::GetInstance()->Random_Int(0, _int(vecTargets.size() - 1))];
		m_iTargetID = pRandomTarget->Get_ObjectID();
	}
	else
		m_iTargetID = -1;
	m_pGameObject->Set_TargetPos(Vec3(_float(m_iTargetID), _float(m_iTargetID), _float(m_iTargetID)));
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Imprisonment_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[0].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext()&& m_iTargetID != -1)
			Add_Prison();
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > 27)
	{
		for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_MONSTER))
		{
			if (pGameObject->Get_ObjectTag() == L"Monster_Prison")
				static_cast<CMonster_Server*>(pGameObject)->Set_Die();	
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Imprisonment_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}

void CValtan_BT_Attack_Imprisonment_Server::Add_Prison()
{
	wstring szComponentName = L"Monster_Prison";
	CMonster_Server::MODELDESC Desc;
	Desc.strFileName = L"Prison";
	Desc.iObjectID = g_iObjectID++;
	Desc.iLayer = (_uint)LAYER_TYPE::LAYER_MONSTER;
	Desc.iLevel = m_pGameObject->Get_CurrLevel();

	Desc.vPosition = CGameInstance::GetInstance()->Find_GameObject(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_PLAYER,m_iTargetID)->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	wstring szMonsterName = L"Prototype_GameObject_" + szComponentName;
	CMonster_Server* pMonster = dynamic_cast<CMonster_Server*>(CGameInstance::GetInstance()->Add_GameObject(m_pGameObject->Get_CurrLevel(), Desc.iLayer, szMonsterName, &Desc));
	if (pMonster == nullptr)
		return;
	Protocol::S_CREATE_OBJCECT tMonsterPkt;

	tMonsterPkt.set_iobjectid(pMonster->Get_ObjectID());
	tMonsterPkt.set_iobjecttype(pMonster->Get_ObjectType());
	tMonsterPkt.set_strname(CAsUtils::ToString(pMonster->Get_ModelName()));
	tMonsterPkt.set_ilayer(pMonster->Get_ObjectLayer());
	tMonsterPkt.set_ilevel(m_pGameObject->Get_CurrLevel());

	tMonsterPkt.set_bcontroll(true);

	auto vPos = tMonsterPkt.mutable_vpos();
	vPos->Resize(3, 0.0f);
	Vec3 vPosition = pMonster->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	memcpy(vPos->mutable_data(), &vPosition, sizeof(Vec3));

	SendBufferRef pSendBuffer = CServerPacketHandler::MakeSendBuffer(tMonsterPkt);
	CGameSessionManager::GetInstance()->Broadcast(pSendBuffer);
}

CValtan_BT_Attack_Imprisonment_Server* CValtan_BT_Attack_Imprisonment_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Imprisonment_Server* pInstance = new CValtan_BT_Attack_Imprisonment_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Imprisonment_Server::Free()
{
	__super::Free();
}
