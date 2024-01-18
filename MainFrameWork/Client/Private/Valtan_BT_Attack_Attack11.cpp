#include "stdafx.h"
#include "Valtan_BT_Attack_Attack11.h"
#include "Boss_Valtan.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack11::CValtan_BT_Attack_Attack11()
{
}

void CValtan_BT_Attack_Attack11::OnStart()
{
	__super::OnStart();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[0].strAnimName, m_vecAnimDesc[0].fChangeTime, m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame, m_vecAnimDesc[0].fAnimSpeed);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack11::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack11::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack11* CValtan_BT_Attack_Attack11::Create(void* pArg)
{
	CValtan_BT_Attack_Attack11* pInstance = new CValtan_BT_Attack_Attack11;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack11::Free()
{
	__super::Free();
}
