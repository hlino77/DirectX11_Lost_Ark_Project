#include "stdafx.h"
#include "Common_BT_Bound_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Bound_Server::CCommon_BT_Bound_Server()
{
}

void CCommon_BT_Bound_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Bound_Server::OnUpdate(_float fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Bound_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Bound(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_SecondHit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_BoundLanding(true);
}



CCommon_BT_Bound_Server* CCommon_BT_Bound_Server::Create(void* pArg)
{
	CCommon_BT_Bound_Server* pInstance = new CCommon_BT_Bound_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Bound_Server::Free()
{
	__super::Free();
}
