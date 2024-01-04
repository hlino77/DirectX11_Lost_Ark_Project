#include "stdafx.h"
#include "Boss_BT_Groggy_Server.h"
#include "Boss_Server.h"
#include "Model.h"
CBoss_BT_Groggy_Server::CBoss_BT_Groggy_Server()
{
}

void CBoss_BT_Groggy_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CBoss_BT_Groggy_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss_Server*>(m_pGameObject)->Get_ArmorDurability() <= 0)
		return BT_FAIL;
	BT_RETURN eReturn = __super::OnUpdate(fTimeDelta);
	if(eReturn ==  BT_SUCCESS)
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	return eReturn;
}

void CBoss_BT_Groggy_Server::OnEnd()
{
	__super::OnEnd();	
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Groggy(false);
}



CBoss_BT_Groggy_Server* CBoss_BT_Groggy_Server::Create(void* pArg)
{
	CBoss_BT_Groggy_Server* pInstance = new CBoss_BT_Groggy_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_Groggy_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_Groggy_Server::Free()
{
	__super::Free();
}
