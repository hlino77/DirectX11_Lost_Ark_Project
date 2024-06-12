#include "stdafx.h"
#include "King_BT_Attack_Charge_Swing_Server.h"
#include "Transform.h"
#include "Model.h"
#include <Boss_Server.h>

CKing_BT_Attack_Charge_Swing_Server::CKing_BT_Attack_Charge_Swing_Server()
{
}

void CKing_BT_Attack_Charge_Swing_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CKing_BT_Attack_Charge_Swing_Server::OnUpdate(_float fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (!static_cast<CBoss_Server*>(m_pGameObject)->Is_GroggyLock() && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && !static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return  __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Charge_Swing_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CKing_BT_Attack_Charge_Swing_Server* CKing_BT_Attack_Charge_Swing_Server::Create(void* pArg)
{
	CKing_BT_Attack_Charge_Swing_Server* pInstance = new CKing_BT_Attack_Charge_Swing_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Charge_Swing_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Charge_Swing_Server::Free()
{
	__super::Free();
}
