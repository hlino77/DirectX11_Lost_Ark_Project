#include "stdafx.h"
#include "Common_BT_Idle_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Idle_Server::CCommon_BT_Idle_Server()
{
}

void CCommon_BT_Idle_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Idle_Server::OnUpdate(_float fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < static_cast<CMonster_Server*>(m_pGameObject)->Get_NoticeRange() && !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;


	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Idle_Server::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Idle_Server* CCommon_BT_Idle_Server::Create(void* pArg)
{
	CCommon_BT_Idle_Server* pInstance = new CCommon_BT_Idle_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Idle_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Idle_Server::Free()
{
	__super::Free();
}
