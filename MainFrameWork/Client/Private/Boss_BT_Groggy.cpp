#include "stdafx.h"
#include "Boss_BT_Groggy.h"
#include "Boss.h"
#include "Model.h"
#include "ColliderSphere.h"

CBoss_BT_Groggy::CBoss_BT_Groggy()
{
}

void CBoss_BT_Groggy::OnStart()
{
	__super::OnStart(0);
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
}

CBT_Node::BT_RETURN CBoss_BT_Groggy::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::VAlTAN)
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#420 (392456203)");
		Add_Sound(0, 1, L"Effect", L"Boss_Down");
		Add_Sound(2, 2, L"Effect", L"Valtan#433 (156533662)");
	}
	else if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::KING)
	{
		Add_Sound(0, 0, L"Effect", L"KING_61");
		Add_Sound(0, 1, L"Effect", L"KING_58");
		Add_Sound(2, 2, L"Effect", L"KING_63");
	}
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
