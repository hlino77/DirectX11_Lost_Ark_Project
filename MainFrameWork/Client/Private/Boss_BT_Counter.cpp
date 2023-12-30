#include "stdafx.h"
#include "Boss_BT_Counter.h"
#include "Boss.h"
#include "Model.h"


CBoss_BT_Counter::CBoss_BT_Counter()
{
}

void CBoss_BT_Counter::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CBoss_BT_Counter::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_Counter::OnEnd()
{
	__super::OnEnd();
}



CBoss_BT_Counter* CBoss_BT_Counter::Create(void* pArg)
{
	CBoss_BT_Counter* pInstance = new CBoss_BT_Counter;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_Counter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_Counter::Free()
{
	__super::Free();
}
