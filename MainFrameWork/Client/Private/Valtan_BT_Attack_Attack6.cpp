#include "stdafx.h"
#include "Valtan_BT_Attack_Attack6.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CValtan_BT_Attack_Attack6::CValtan_BT_Attack_Attack6()
{
}

void CValtan_BT_Attack_Attack6::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack6::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack6::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack6* CValtan_BT_Attack_Attack6::Create(void* pArg)
{
	CValtan_BT_Attack_Attack6* pInstance = new CValtan_BT_Attack_Attack6;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack6::Free()
{
	__super::Free();
}
