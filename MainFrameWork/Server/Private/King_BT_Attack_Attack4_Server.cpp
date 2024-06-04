#include "stdafx.h"
#include "King_BT_Attack_Attack4_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CKing_BT_Attack_Attack4_Server::CKing_BT_Attack_Attack4_Server()
{
}

void CKing_BT_Attack_Attack4_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CKing_BT_Attack_Attack4_Server::OnUpdate(_float fTimeDelta)
{
	if (m_IsTeleport && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		Vec3 vTargetPos = static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTarget_Position();
		Vec3 vDir = static_cast<CMonster_Server*>(m_pGameObject)->Get_Target_Direction();
		vDir.Normalize();
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vTargetPos - vDir * 0.75f);
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction();
		m_IsTeleport = false;
	}
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);
	return  __super::OnUpdate(fTimeDelta);;
}

void CKing_BT_Attack_Attack4_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AttackRange(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CKing_BT_Attack_Attack4_Server* CKing_BT_Attack_Attack4_Server::Create(void* pArg)
{
	CKing_BT_Attack_Attack4_Server* pInstance = new CKing_BT_Attack_Attack4_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Attack4_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Attack4_Server::Free()
{
	__super::Free();
}
