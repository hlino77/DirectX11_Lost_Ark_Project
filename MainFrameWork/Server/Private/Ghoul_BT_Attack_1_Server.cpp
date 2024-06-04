#include "stdafx.h"
#include "Ghoul_BT_Attack_1_Server.h"
#include "Monster_Server.h"
#include "Transform.h"
#include "Model.h"

CGhoul_BT_Attack_1_Server::CGhoul_BT_Attack_1_Server()
{
}

void CGhoul_BT_Attack_1_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CGhoul_BT_Attack_1_Server::OnUpdate(_float fTimeDelta)
{

	return  __super::OnUpdate(fTimeDelta);
}

void CGhoul_BT_Attack_1_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(1);
	static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
}



CGhoul_BT_Attack_1_Server* CGhoul_BT_Attack_1_Server::Create(void* pArg)
{
	CGhoul_BT_Attack_1_Server* pInstance = new CGhoul_BT_Attack_1_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGhoul_BT_Attack_1_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGhoul_BT_Attack_1_Server::Free()
{
	__super::Free();
}
