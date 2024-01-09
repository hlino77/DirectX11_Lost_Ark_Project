#include "stdafx.h"
#include "Valtan_BT_Attack_Attack2.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_Attack2::CValtan_BT_Attack_Attack2()
{
}

void CValtan_BT_Attack_Attack2::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[0].strAnimName, m_vecAnimDesc[0].fChangeTime, m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack2::OnUpdate(const _float& fTimeDelta)
{

	return  __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack2::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack2* CValtan_BT_Attack_Attack2::Create(void* pArg)
{
	CValtan_BT_Attack_Attack2* pInstance = new CValtan_BT_Attack_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack2::Free()
{
	__super::Free();
}
