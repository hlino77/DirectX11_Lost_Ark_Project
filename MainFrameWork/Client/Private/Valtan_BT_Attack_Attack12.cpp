#include "stdafx.h"
#include "Valtan_BT_Attack_Attack12.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack12::CValtan_BT_Attack_Attack12()
{
}

void CValtan_BT_Attack_Attack12::OnStart()
{
	__super::OnStart();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack12::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 55 && m_iCurrAnimation == 0)
	{
		m_iCurrAnimation = 1;

		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,
			m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame, m_vecAnimDesc[1].fRootDist, m_vecAnimDesc[1].IsRootRot);
	}
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) < 35&& m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iCurrAnimation].iAnimIndex && m_iCurrAnimation != 3)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack12::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack12* CValtan_BT_Attack_Attack12::Create(void* pArg)
{
	CValtan_BT_Attack_Attack12* pInstance = new CValtan_BT_Attack_Attack12;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack12::Free()
{
	__super::Free();
}
