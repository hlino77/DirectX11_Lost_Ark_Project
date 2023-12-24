#include "stdafx.h"
#include "King_BT_Attack_Attack2_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"

CKing_BT_Attack_Attack2_Server::CKing_BT_Attack_Attack2_Server()
{
}

void CKing_BT_Attack_Attack2_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack2_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimIndexFrame[0].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction();
	return __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Attack2_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(1);
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CKing_BT_Attack_Attack2_Server* CKing_BT_Attack_Attack2_Server::Create(void* pArg)
{
	CKing_BT_Attack_Attack2_Server* pInstance = new CKing_BT_Attack_Attack2_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack2_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack2_Server::Free()
{
	__super::Free();
}
