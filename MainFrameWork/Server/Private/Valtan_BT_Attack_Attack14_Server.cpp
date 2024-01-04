#include "stdafx.h"
#include "Valtan_BT_Attack_Attack14_Server.h"
#include "Boss_Server.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack14_Server::CValtan_BT_Attack_Attack14_Server()
{
}

void CValtan_BT_Attack_Attack14_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack14_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss_Server*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack14_Server::OnEnd()
{
	__super::OnEnd();
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase() == 1)
		static_cast<CMonster_Server*>(m_pGameObject)->Reset_SkillStack();
	else
		static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack14_Server* CValtan_BT_Attack_Attack14_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack14_Server* pInstance = new CValtan_BT_Attack_Attack14_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack14_Server::Free()
{
	__super::Free();
}
