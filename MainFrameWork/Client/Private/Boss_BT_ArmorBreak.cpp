#include "stdafx.h"
#include "Boss_BT_ArmorBreak.h"
#include "Boss.h"
#include "Model.h"
#include "Damage_Manager.h"

CBoss_BT_ArmorBreak::CBoss_BT_ArmorBreak()
{
}

void CBoss_BT_ArmorBreak::OnStart()
{
	__super::OnStart(0);
	CDamage_Manager::GetInstance()->Print_DamageFont(m_pGameObject, 0.8f, 0.f, 2.0f, true, 1);
	static_cast<CBoss*>(m_pGameObject)->Set_Armor(static_cast<CBoss*>(m_pGameObject)->Get_Armor() - 1);
}

CBT_Node::BT_RETURN CBoss_BT_ArmorBreak::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::BOSS_TYPE::VAlTAN)
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#422 (793019468)");
		Add_Sound(0, 1, L"Effect", L"Valtan#3 (97351323)");
		Add_Sound(2, 2, L"Effect", L"Valtan#433 (156533662)");
	}
	return __super::OnUpdate(fTimeDelta);
}

void CBoss_BT_ArmorBreak::OnEnd()
{
	__super::OnEnd();
}



CBoss_BT_ArmorBreak* CBoss_BT_ArmorBreak::Create(void* pArg)
{
	CBoss_BT_ArmorBreak* pInstance = new CBoss_BT_ArmorBreak;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CBoss_BT_ArmorBreak");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBoss_BT_ArmorBreak::Free()
{
	__super::Free();
}
