#include "stdafx.h"
#include "Valtan_BT_Attack_Attack24_Server.h"
#include "Monster_Server.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan_Server.h>

CValtan_BT_Attack_Attack24_Server::CValtan_BT_Attack_Attack24_Server()
{
}

void CValtan_BT_Attack_Attack24_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack24_Server::OnUpdate(const _float& fTimeDelta)
{
	
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack24_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CBoss_Valtan_Server*>(m_pGameObject)->Clear_GrabbedPlayerIDs();
}



CValtan_BT_Attack_Attack24_Server* CValtan_BT_Attack_Attack24_Server::Create(void* pArg)
{
	CValtan_BT_Attack_Attack24_Server* pInstance = new CValtan_BT_Attack_Attack24_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack24_Server::Free()
{
	__super::Free();
}
