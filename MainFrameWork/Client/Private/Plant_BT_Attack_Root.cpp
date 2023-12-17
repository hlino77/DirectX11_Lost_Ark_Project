#include "stdafx.h"
#include "Plant_BT_Attack_Root.h"
#include "Monster.h"
#include "Model.h"
CPlant_BT_Attack_Root::CPlant_BT_Attack_Root()
{
}

void CPlant_BT_Attack_Root::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CPlant_BT_Attack_Root::OnUpdate(const _float& fTimeDelta)
{


	return BT_RUNNING;
}

void CPlant_BT_Attack_Root::OnEnd()
{
	__super::OnEnd();

}



CPlant_BT_Attack_Root* CPlant_BT_Attack_Root::Create(void* pArg)
{
	CPlant_BT_Attack_Root* pInstance = new CPlant_BT_Attack_Root;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPlant_BT_Attack_Root");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_BT_Attack_Root::Free()
{
	__super::Free();
}
