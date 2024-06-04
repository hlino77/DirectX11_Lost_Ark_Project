#include "stdafx.h"
#include "Golem_BT_Attack_Charge_Punch_Server.h"
#include "Model.h"
#include <Boss_Server.h>

CGolem_BT_Attack_Charge_Punch_Server::CGolem_BT_Attack_Charge_Punch_Server()
{
}

void CGolem_BT_Attack_Charge_Punch_Server::OnStart()
{
	__super::OnStart(0);	
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Charge_Punch_Server::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (!static_cast<CBoss_Server*>(m_pGameObject)->Is_GroggyLock() && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && !static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	};

	return  __super::OnUpdate(fTimeDelta);
}

void CGolem_BT_Attack_Charge_Punch_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CGolem_BT_Attack_Charge_Punch_Server* CGolem_BT_Attack_Charge_Punch_Server::Create(void* pArg)
{
	CGolem_BT_Attack_Charge_Punch_Server* pInstance = new CGolem_BT_Attack_Charge_Punch_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Charge_Punch_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Charge_Punch_Server::Free()
{
	__super::Free();
}
