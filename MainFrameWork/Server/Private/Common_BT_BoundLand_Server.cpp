#include "stdafx.h"
#include "Common_BT_BoundLand_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CCommon_BT_BoundLand_Server::CCommon_BT_BoundLand_Server()
{
}

void CCommon_BT_BoundLand_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CCommon_BT_BoundLand_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_SUCCESS;

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_BoundLand_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_BoundLanding(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Downed(true);
}



CCommon_BT_BoundLand_Server* CCommon_BT_BoundLand_Server::Create(void* pArg)
{
	CCommon_BT_BoundLand_Server* pInstance = new CCommon_BT_BoundLand_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_BoundLand_Server::Free()
{
	__super::Free();
}
