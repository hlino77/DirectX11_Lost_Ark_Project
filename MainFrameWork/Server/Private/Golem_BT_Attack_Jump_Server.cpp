#include "stdafx.h"
#include "Golem_BT_Attack_Jump_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CGolem_BT_Attack_Jump_Server::CGolem_BT_Attack_Jump_Server()
{
}

void CGolem_BT_Attack_Jump_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();

}

CBT_Node::BT_RETURN CGolem_BT_Attack_Jump_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if(m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CGolem_BT_Attack_Jump_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CGolem_BT_Attack_Jump_Server* CGolem_BT_Attack_Jump_Server::Create(void* pArg)
{
	CGolem_BT_Attack_Jump_Server* pInstance = new CGolem_BT_Attack_Jump_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Jump_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Jump_Server::Free()
{
	__super::Free();
}
