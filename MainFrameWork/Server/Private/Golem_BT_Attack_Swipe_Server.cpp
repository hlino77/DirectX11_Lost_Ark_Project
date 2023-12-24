#include "stdafx.h"
#include "Golem_BT_Attack_Swipe_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CGolem_BT_Attack_Swipe_Server::CGolem_BT_Attack_Swipe_Server()
{
}

void CGolem_BT_Attack_Swipe_Server::OnStart()
{
	__super::OnStart(0);

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Swipe_Server::OnUpdate(const _float& fTimeDelta)
{

if(m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CGolem_BT_Attack_Swipe_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CGolem_BT_Attack_Swipe_Server* CGolem_BT_Attack_Swipe_Server::Create(void* pArg)
{
	CGolem_BT_Attack_Swipe_Server* pInstance = new CGolem_BT_Attack_Swipe_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Swipe_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Swipe_Server::Free()
{
	__super::Free();
}
