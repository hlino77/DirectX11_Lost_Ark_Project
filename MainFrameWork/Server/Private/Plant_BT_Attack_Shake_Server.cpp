#include "stdafx.h"
#include "Plant_BT_Attack_Shake_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CPlant_BT_Attack_Shake_Server::CPlant_BT_Attack_Shake_Server()
{
}

void CPlant_BT_Attack_Shake_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CPlant_BT_Attack_Shake_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if(m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CPlant_BT_Attack_Shake_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(1);

}



CPlant_BT_Attack_Shake_Server* CPlant_BT_Attack_Shake_Server::Create(void* pArg)
{
	CPlant_BT_Attack_Shake_Server* pInstance = new CPlant_BT_Attack_Shake_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Shake_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_BT_Attack_Shake_Server::Free()
{
	__super::Free();
}
