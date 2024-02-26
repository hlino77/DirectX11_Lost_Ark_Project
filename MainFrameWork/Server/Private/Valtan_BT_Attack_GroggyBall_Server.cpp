#include "stdafx.h"
#include "Valtan_BT_Attack_GroggyBall_Server.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_GroggyBall_Server::CValtan_BT_Attack_GroggyBall_Server()
{
}

void CValtan_BT_Attack_GroggyBall_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	if (m_bFirst)
	{
		m_bFirst = false;
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
	}
	m_bShield = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_GroggyBall_Server::OnUpdate(const _float& fTimeDelta)
{

	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_SkipAction())
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_SkipAction(false);
		return BT_SUCCESS;
	}
	if (!m_bShield && m_iCurrAnimation == 1 && !m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxGroggyCount() == 0)
	{
		m_bShield = true;
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Shield(20902324);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_MaxGroggyCount(150);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxGroggyCount());
	}
	if (m_iCurrAnimation == 2 && !m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxGroggyCount() > 0)
	{
		static_cast<CBoss_Server*>(m_pGameObject)->Set_Invincible(true);
		static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Set_Shield(0);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_MaxGroggyCount(0);
		static_cast<CBoss_Server*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxGroggyCount());
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_GroggyBall_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Invincible(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
}

void CValtan_BT_Attack_GroggyBall_Server::On_FirstAnimStart()
{
}

void CValtan_BT_Attack_GroggyBall_Server::On_LastAnimEnd()
{
}



CValtan_BT_Attack_GroggyBall_Server* CValtan_BT_Attack_GroggyBall_Server::Create(void* pArg)
{
	CValtan_BT_Attack_GroggyBall_Server* pInstance = new CValtan_BT_Attack_GroggyBall_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_GroggyBall_Server::Free()
{
	__super::Free();
}
