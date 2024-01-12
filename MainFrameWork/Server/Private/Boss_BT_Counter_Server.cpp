#include "stdafx.h"
#include "Boss_BT_Counter_Server.h"
#include "Boss_Server.h"
#include "Model.h"
CBoss_BT_Counter_Server::CBoss_BT_Counter_Server()
{
}

void CBoss_BT_Counter_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CBoss_BT_Counter_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_Hp() < 1)
		return BT_FAIL;
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_Counter_Server::OnEnd()
{
	__super::OnEnd();
	if (m_eReturn == BT_SUCCESS)
		static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Countered(false);
}



CBoss_BT_Counter_Server* CBoss_BT_Counter_Server::Create(void* pArg)
{
	CBoss_BT_Counter_Server* pInstance = new CBoss_BT_Counter_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_Counter_Server::Free()
{
	__super::Free();
}
