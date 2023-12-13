#include "stdafx.h"
#include "Zombie_BT_Spawn_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CZombie_BT_Spawn_Server::CZombie_BT_Spawn_Server()
{
}

void CZombie_BT_Spawn_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Action(m_strActionName);
	static_cast<CMonster_Server*>(m_pGameObject)->Send_Monster_Action();
}

CBT_Node::BT_RETURN CZombie_BT_Spawn_Server::OnUpdate(const _float& fTimeDelta)
{
	if (IsOut())
		return BT_FAIL;
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Spawn_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Spawn(false);
}

_bool CZombie_BT_Spawn_Server::IsOut()
{
	if (static_cast<CMonster_Server*>(m_pGameObject)->Is_Hit())
		return true;

	return false;
}

CZombie_BT_Spawn_Server* CZombie_BT_Spawn_Server::Create(void* pArg)
{
	CZombie_BT_Spawn_Server* pInstance = new CZombie_BT_Spawn_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Spawn_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Spawn_Server::Free()
{
	__super::Free();
}
