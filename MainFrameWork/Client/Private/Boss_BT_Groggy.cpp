#include "stdafx.h"
#include "Boss_BT_Groggy.h"
#include "Boss.h"
#include "Model.h"
CBoss_BT_Groggy::CBoss_BT_Groggy()
{
}

void CBoss_BT_Groggy::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CBoss_BT_Groggy::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_Groggy::OnEnd()
{
	__super::OnEnd();
	if (static_cast<CBoss*>(m_pGameObject)->Get_GroggyGauge() < 1)
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyGauge(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyGauge());
}



CBoss_BT_Groggy* CBoss_BT_Groggy::Create(void* pArg)
{
	CBoss_BT_Groggy* pInstance = new CBoss_BT_Groggy;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_Groggy");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_Groggy::Free()
{
	__super::Free();
}
