#include "stdafx.h"
#include "Pawn_BT_Attack2_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CPawn_BT_Attack2_Server::CPawn_BT_Attack2_Server()
{
}

void CPawn_BT_Attack2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CPawn_BT_Attack2_Server::OnUpdate(const _float& fTimeDelta)
{

	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CPawn_BT_Attack2_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(0);
}



CPawn_BT_Attack2_Server* CPawn_BT_Attack2_Server::Create(void* pArg)
{
	CPawn_BT_Attack2_Server* pInstance = new CPawn_BT_Attack2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPawn_BT_Attack2_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPawn_BT_Attack2_Server::Free()
{
	__super::Free();
}
