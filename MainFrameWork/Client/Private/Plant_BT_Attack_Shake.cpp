#include "stdafx.h"
#include "Plant_BT_Attack_Shake.h"
#include "Monster.h"
#include "Model.h"
CPlant_BT_Attack_Shake_Server::CPlant_BT_Attack_Shake_Server()
{
}

void CPlant_BT_Attack_Shake_Server::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_Action(m_strActionName);

}

CBT_Node::BT_RETURN CPlant_BT_Attack_Shake_Server::OnUpdate(const _float& fTimeDelta)
{

	return BT_RUNNING;
}

void CPlant_BT_Attack_Shake_Server::OnEnd()
{
	__super::OnEnd();

}



CPlant_BT_Attack_Shake_Server* CPlant_BT_Attack_Shake_Server::Create(void* pArg)
{
	CPlant_BT_Attack_Shake_Server* pInstance = new CPlant_BT_Attack_Shake_Server;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPlant_BT_Attack_Shake_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_BT_Attack_Shake_Server::Free()
{
	__super::Free();
}
