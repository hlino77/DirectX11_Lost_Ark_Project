#include "stdafx.h"
#include "Valtan_BT_Attack_Attack15_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_Attack15_Server::CValtan_BT_Attack_Attack15_Server()
{
}

void CValtan_BT_Attack_Attack15_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack15_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Counter() || static_cast<CBoss_Server*>(m_pGameObject)->Get_Grogginess())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_HitCount(0);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Get_HitCount() > 20)
	{
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Hit(true);
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Grogginess(true);
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Groggy(true);
		return BT_FAIL;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack15_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack15_Server* CValtan_BT_Attack_Attack15_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack15_Server* pInstance = new CValtan_BT_Attack_Attack15_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack15_Server::Free()
{
	__super::Free();
}
