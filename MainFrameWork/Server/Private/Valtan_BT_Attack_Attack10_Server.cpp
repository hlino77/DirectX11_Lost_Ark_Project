#include "stdafx.h"
#include "Valtan_BT_Attack_Attack10_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Server.h>

CValtan_BT_Attack_Attack10_Server::CValtan_BT_Attack_Attack10_Server()
{
}

void CValtan_BT_Attack_Attack10_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack10_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Counter() || static_cast<CBoss_Server*>(m_pGameObject)->Get_Grogginess())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Counter(false);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Grogginess(false);
		return BT_SUCCESS;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[2].iAnimIndex)
		static_cast<CMonster_Server*>(m_pGameObject)->Move_Dir(static_cast<CMonster_Server*>(m_pGameObject)->Get_Target_Direction(),
			0.4f * static_cast<CMonster_Server*>(m_pGameObject)->Get_MoveSpeed(),fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) < 10 || m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack10_Server::OnEnd()
{
	__super::OnEnd();	
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_Phase() != 1)
		static_cast<CMonster_Server*>(m_pGameObject)->Add_SkillStack();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}



CValtan_BT_Attack_Attack10_Server* CValtan_BT_Attack_Attack10_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack10_Server* pInstance = new CValtan_BT_Attack_Attack10_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack10_Server::Free()
{
	__super::Free();
}
