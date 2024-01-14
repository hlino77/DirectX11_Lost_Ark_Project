#include "stdafx.h"
#include "Boss_BT_ArmorBreak_Server.h"
#include "Boss_Server.h"
#include "Model.h"
CBoss_BT_ArmorBreak_Server::CBoss_BT_ArmorBreak_Server()
{
}

void CBoss_BT_ArmorBreak_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_Armor(max(static_cast<CBoss_Server*>(m_pGameObject)->Get_Armor() - 1,0));
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CBoss_BT_ArmorBreak_Server::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_Hp() < 1)
		return BT_FAIL;
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_ArmorBreak_Server::OnEnd()
{
	__super::OnEnd();

	static_cast<CMonster_Server*>(m_pGameObject)->Set_Attacked(true);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CBoss_Server*>(m_pGameObject)->Set_ArmorDurability(static_cast<CBoss_Server*>(m_pGameObject)->Get_MaxArmorDurability());

}



CBoss_BT_ArmorBreak_Server* CBoss_BT_ArmorBreak_Server::Create(void* pArg)
{
	CBoss_BT_ArmorBreak_Server* pInstance = new CBoss_BT_ArmorBreak_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_ArmorBreak_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_ArmorBreak_Server::Free()
{
	__super::Free();
}
