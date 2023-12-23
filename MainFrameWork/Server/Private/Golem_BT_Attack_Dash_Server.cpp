#include "stdafx.h"
#include "Golem_BT_Attack_Dash_Server.h"
#include "Monster_Server.h"
#include "Transform.h"
#include "Model.h"
CGolem_BT_Attack_Dash_Server::CGolem_BT_Attack_Dash_Server()
{
}

void CGolem_BT_Attack_Dash_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Dash_Server::OnUpdate(const _float& fTimeDelta)
{

	if (m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrameRatio(m_vecAnimIndexFrame[0].iAnimIndex)>0.8f)
		return BT_SUCCESS;
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[0].iAnimIndex) < 10)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return BT_RUNNING;
}

void CGolem_BT_Attack_Dash_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CGolem_BT_Attack_Dash_Server* CGolem_BT_Attack_Dash_Server::Create(void* pArg)
{
	CGolem_BT_Attack_Dash_Server* pInstance = new CGolem_BT_Attack_Dash_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Dash_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Dash_Server::Free()
{
	__super::Free();
}
