#include "stdafx.h"
#include "Plant_BT_Attack_Root.h"
#include "Monster.h"
#include "Model.h"
CPlant_BT_Attack_Root_Server::CPlant_BT_Attack_Root_Server()
{
}

void CPlant_BT_Attack_Root_Server::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CPlant_BT_Attack_Root_Server::OnUpdate(const _float& fTimeDelta)
{


	return BT_RUNNING;
}

void CPlant_BT_Attack_Root_Server::OnEnd()
{
	__super::OnEnd();

}



CPlant_BT_Attack_Root_Server* CPlant_BT_Attack_Root_Server::Create(void* pArg)
{
	CPlant_BT_Attack_Root_Server* pInstance = new CPlant_BT_Attack_Root_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPlant_BT_Attack_Root_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_BT_Attack_Root_Server::Free()
{
	__super::Free();
}
