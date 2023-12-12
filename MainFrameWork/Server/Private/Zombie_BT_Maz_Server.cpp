#include "stdafx.h"
#include "Zombie_BT_Maz_Server.h"
#include "Monster_Server.h"
#include "Model.h"
CZombie_BT_Maz_Server::CZombie_BT_Maz_Server()
{
}

void CZombie_BT_Maz_Server::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Maz_Server::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Maz_Server::OnEnd()
{
	__super::OnEnd();
}



CZombie_BT_Maz_Server* CZombie_BT_Maz_Server::Create(void* pArg)
{
	CZombie_BT_Maz_Server* pInstance = new CZombie_BT_Maz_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Maz_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Maz_Server::Free()
{
	__super::Free();
}
