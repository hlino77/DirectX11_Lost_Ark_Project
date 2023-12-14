#include "stdafx.h"
#include "Zombie_BT_Move_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CZombie_BT_Move_Server::CZombie_BT_Move_Server()
{
}

void CZombie_BT_Move_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_RandomPosition();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CZombie_BT_Move_Server::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if (static_cast<CMonster_Server*>(m_pGameObject)->Get_NearTargetDistance() < 10.f && !static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;

	if(static_cast<CMonster_Server*>(m_pGameObject)->Is_Close_To_RandomPosition())
		return BT_FAIL;

	static_cast<CMonster_Server*>(m_pGameObject)->Move_to_RandomPosition(fTimeDelta);

	return BT_RUNNING;
}

void CZombie_BT_Move_Server::OnEnd()
{
	__super::OnEnd();
}



CZombie_BT_Move_Server* CZombie_BT_Move_Server::Create(void* pArg)
{
	CZombie_BT_Move_Server* pInstance = new CZombie_BT_Move_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Move_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Move_Server::Free()
{
	__super::Free();
}
