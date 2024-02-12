#include "stdafx.h"
#include "Common_BT_BattleIdle_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_BattleIdle_Server::CCommon_BT_BattleIdle_Server()
{
}

void CCommon_BT_BattleIdle_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
	m_fIdletime = 0.f;
}

CBT_Node::BT_RETURN CCommon_BT_BattleIdle_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	static_cast<CMonster_Server*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	m_fIdletime += fTimeDelta;

	if (m_fIdletime> 2.f)
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CCommon_BT_BattleIdle_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(false);
	m_fIdletime = 0.f;
}



CCommon_BT_BattleIdle_Server* CCommon_BT_BattleIdle_Server::Create(void* pArg)
{
	CCommon_BT_BattleIdle_Server* pInstance = new CCommon_BT_BattleIdle_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_BattleIdle_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_BattleIdle_Server::Free()
{
	__super::Free();
}
