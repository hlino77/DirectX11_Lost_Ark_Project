#include "stdafx.h"
#include "Plant_BT_Attack_Shake.h"
#include "Monster.h"
#include "Model.h"
CPlant_BT_Attack_Shake::CPlant_BT_Attack_Shake()
{
}

void CPlant_BT_Attack_Shake::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Set_Action(m_strActionName);

}

CBT_Node::BT_RETURN CPlant_BT_Attack_Shake::OnUpdate(const _float& fTimeDelta)
{

	return BT_RUNNING;
}

void CPlant_BT_Attack_Shake::OnEnd()
{
	__super::OnEnd();

}



CPlant_BT_Attack_Shake* CPlant_BT_Attack_Shake::Create(void* pArg)
{
	CPlant_BT_Attack_Shake* pInstance = new CPlant_BT_Attack_Shake;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CPlant_BT_Attack_Shake");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlant_BT_Attack_Shake::Free()
{
	__super::Free();
}
