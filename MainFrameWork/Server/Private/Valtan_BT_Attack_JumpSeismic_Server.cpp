#include "stdafx.h"
#include "Valtan_BT_Attack_JumpSeismic_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CValtan_BT_Attack_JumpSeismic_Server::CValtan_BT_Attack_JumpSeismic_Server()
{
}

void CValtan_BT_Attack_JumpSeismic_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_JumpSeismic_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction() )
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Target_Distance() > 0.2f)
			static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction();
		static_cast<CBoss_Server*>(m_pGameObject)->Move_Dir(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK),
			0.7f * static_cast<CBoss_Server*>(m_pGameObject)->Get_MoveSpeed(), fTimeDelta);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_JumpSeismic_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_JumpSeismic_Server* CValtan_BT_Attack_JumpSeismic_Server::Create(void* pArg)
{
	CValtan_BT_Attack_JumpSeismic_Server* pInstance = new CValtan_BT_Attack_JumpSeismic_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_JumpSeismic_Server::Free()
{
	__super::Free();
}
