#include "stdafx.h"
#include "Golem_BT_Attack_Charge_Punch_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CGolem_BT_Attack_Charge_Punch_Server::CGolem_BT_Attack_Charge_Punch_Server()
{
}

void CGolem_BT_Attack_Charge_Punch_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Charge_Punch_Server::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CGolem_BT_Attack_Charge_Punch_Server::OnEnd()
{
	__super::OnEnd();
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
