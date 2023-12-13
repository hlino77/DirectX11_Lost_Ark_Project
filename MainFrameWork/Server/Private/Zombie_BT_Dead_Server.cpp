#include "stdafx.h"
#include "Zombie_BT_Dead_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CZombie_BT_Dead_Server::CZombie_BT_Dead_Server()
{
}

void CZombie_BT_Dead_Server::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CZombie_BT_Dead_Server::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CZombie_BT_Dead_Server::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster_Server*>(m_pGameObject)->Set_Die();
}



CZombie_BT_Dead_Server* CZombie_BT_Dead_Server::Create(void* pArg)
{
	CZombie_BT_Dead_Server* pInstance = new CZombie_BT_Dead_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Dead_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Dead_Server::Free()
{
	__super::Free();
}
