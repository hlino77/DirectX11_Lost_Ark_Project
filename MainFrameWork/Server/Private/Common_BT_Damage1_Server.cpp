#include "stdafx.h"
#include "Common_BT_Damage1_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "BehaviorTree.h"
CCommon_BT_Damage1_Server::CCommon_BT_Damage1_Server()
{
}

void CCommon_BT_Damage1_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_fDuration = 0.9f;
}

CBT_Node::BT_RETURN CCommon_BT_Damage1_Server::OnUpdate(_float fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Bound() || static_cast<CMonster_Server*>(m_pGameObject)->Is_Twist() || m_pGameObject->Get_Hp() < 1||static_cast<CMonster_Server*>(m_pGameObject)->Is_Maz())
	{
		return BT_SUCCESS;
	}

	if (m_fDuration < 0.f)
	{
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
		static_cast<CMonster_Server*>(m_pGameObject)->Set_SecondHit(false);
		return BT_SUCCESS;
	}

	m_fDuration -= fTimeDelta;
	return BT_RUNNING;
}

void CCommon_BT_Damage1_Server::OnEnd()
{
	__super::OnEnd();

}



CCommon_BT_Damage1_Server* CCommon_BT_Damage1_Server::Create(void* pArg)
{
	CCommon_BT_Damage1_Server* pInstance = new CCommon_BT_Damage1_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Damage1_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Damage1_Server::Free()
{
	__super::Free();
}
