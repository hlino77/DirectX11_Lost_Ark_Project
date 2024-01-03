#include "stdafx.h"
#include "Valtan_BT_Attack_Attack23_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack23_Server::CValtan_BT_Attack_Attack23_Server()
{
}

void CValtan_BT_Attack_Attack23_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack23_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
		static_cast<CBoss_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_iCurrAnimation == m_iMaxAnimation - 1)
		m_pGameObject->Set_Invincible(true);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack23_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack23_Server* CValtan_BT_Attack_Attack23_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack23_Server* pInstance = new CValtan_BT_Attack_Attack23_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack23_Server::Free()
{
	__super::Free();
}
