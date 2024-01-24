#include "stdafx.h"
#include "Valtan_BT_Attack_Attack2_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_Attack2_Server::CValtan_BT_Attack_Attack2_Server()
{
}

void CValtan_BT_Attack_Attack2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack2_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
	{
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_GroggyObsrob(100);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Get_GroggyObsrob() < 1)
	{
		return BT_FAIL;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack2_Server::OnEnd()
{
	__super::OnEnd();
	if (m_eReturn == BT_SUCCESS)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	}
}



CValtan_BT_Attack_Attack2_Server* CValtan_BT_Attack_Attack2_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack2_Server* pInstance = new CValtan_BT_Attack_Attack2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack2_Server::Free()
{
	__super::Free();
}
