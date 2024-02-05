#include "stdafx.h"
#include "Valtan_BT_Attack_TrippleCounterChop_1_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CValtan_BT_Attack_TrippleCounterChop_1_Server::CValtan_BT_Attack_TrippleCounterChop_1_Server()
{
}

void CValtan_BT_Attack_TrippleCounterChop_1_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_TrippleCounterChop_1_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex &&m_fLoopTime>1.8f &&!static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss_Server*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss_Server*>(m_pGameObject)->Set_CounterSkill(false);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_TrippleCounterChop_1_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}

void CValtan_BT_Attack_TrippleCounterChop_1_Server::On_FirstAnimStart()
{
	static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyLock(true);
}



CValtan_BT_Attack_TrippleCounterChop_1_Server* CValtan_BT_Attack_TrippleCounterChop_1_Server::Create(void* pArg)
{
	CValtan_BT_Attack_TrippleCounterChop_1_Server* pInstance = new CValtan_BT_Attack_TrippleCounterChop_1_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_TrippleCounterChop_1_Server::Free()
{
	__super::Free();
}
