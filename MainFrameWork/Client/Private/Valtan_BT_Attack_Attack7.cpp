#include "stdafx.h"
#include "Valtan_BT_Attack_Attack7.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack7::CValtan_BT_Attack_Attack7()
{
}

void CValtan_BT_Attack_Attack7::OnStart()
{
	__super::OnStart();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack7::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 80 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex,
			m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame,
			m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack7::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack7* CValtan_BT_Attack_Attack7::Create(void* pArg)
{
	CValtan_BT_Attack_Attack7* pInstance = new CValtan_BT_Attack_Attack7;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack7::Free()
{
	__super::Free();
}
