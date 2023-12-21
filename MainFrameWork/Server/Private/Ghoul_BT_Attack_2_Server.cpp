#include "stdafx.h"
#include "Ghoul_BT_Attack_2_Server.h"
#include "Monster_Server.h"
#include "Transform.h"
#include "Model.h"
CGhoul_BT_Attack_2_Server::CGhoul_BT_Attack_2_Server()
{
}

void CGhoul_BT_Attack_2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Boss_Action();
}

CBT_Node::BT_RETURN CGhoul_BT_Attack_2_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CGhoul_BT_Attack_2_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);

	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CGhoul_BT_Attack_2_Server* CGhoul_BT_Attack_2_Server::Create(void* pArg)
{
	CGhoul_BT_Attack_2_Server* pInstance = new CGhoul_BT_Attack_2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_2_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_2_Server::Free()
{
	__super::Free();
}
