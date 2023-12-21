#include "stdafx.h"
#include "Common_BT_Land_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_Land_Server::CCommon_BT_Land_Server()
{
}

void CCommon_BT_Land_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_Land_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_SecondHit() && static_cast<CMonster_Server*>(m_pGameObject)->Get_SecondHitForce() >= 30)
		return BT_SUCCESS;

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Land_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_Land_Server* CCommon_BT_Land_Server::Create(void* pArg)
{
	CCommon_BT_Land_Server* pInstance = new CCommon_BT_Land_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Land_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Land_Server::Free()
{
	__super::Free();
}
