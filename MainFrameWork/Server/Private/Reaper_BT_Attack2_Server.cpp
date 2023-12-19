#include "stdafx.h"
#include "Reaper_BT_Attack2_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CReaper_BT_Attack2_Server::CReaper_BT_Attack2_Server()
{
}

void CReaper_BT_Attack2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CReaper_BT_Attack2_Server::OnUpdate(const _float& fTimeDelta)
{

	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CReaper_BT_Attack2_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(0);
}



CReaper_BT_Attack2_Server* CReaper_BT_Attack2_Server::Create(void* pArg)
{
	CReaper_BT_Attack2_Server* pInstance = new CReaper_BT_Attack2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CReaper_BT_Attack2_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CReaper_BT_Attack2_Server::Free()
{
	__super::Free();
}
