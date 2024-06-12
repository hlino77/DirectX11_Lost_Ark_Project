#include "stdafx.h"
#include "Valtan_BT_Attack_ChainDestructionFist_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_ChainDestructionFist_Server::CValtan_BT_Attack_ChainDestructionFist_Server()
{
}

void CValtan_BT_Attack_ChainDestructionFist_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_SetuponCell(false);

	m_vTargetPos = Vec3(100.0f, 0.19f, 100.0f);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_ChainDestructionFist_Server::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Set_SetuponCell(true);
		static_cast<CBoss_Server*>(m_pGameObject)->Get_TransformCom()->LookAt_Dir(Vec3(0.f,0.f,-1.f));
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vTargetPos);
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) >= 95 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);

	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_ChainDestructionFist_Server::OnEnd()
{
	__super::OnEnd();
}

void CValtan_BT_Attack_ChainDestructionFist_Server::On_FirstAnimStart()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
}

void CValtan_BT_Attack_ChainDestructionFist_Server::On_LastAnimEnd()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
}



CValtan_BT_Attack_ChainDestructionFist_Server* CValtan_BT_Attack_ChainDestructionFist_Server::Create(void* pArg)
{
	CValtan_BT_Attack_ChainDestructionFist_Server* pInstance = new CValtan_BT_Attack_ChainDestructionFist_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_ChainDestructionFist_Server::Free()
{
	__super::Free();
}
