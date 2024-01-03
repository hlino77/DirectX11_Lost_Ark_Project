#include "stdafx.h"
#include "Valtan_BT_Attack_Attack13.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"

CValtan_BT_Attack_Attack13::CValtan_BT_Attack_Attack13()
{
}

void CValtan_BT_Attack_Attack13::OnStart()
{
	__super::OnStart(0);
	m_bOutSide = m_pGameObject->Get_TargetPos().x;


}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack13::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack13::OnEnd()
{
	__super::OnEnd();

}



CValtan_BT_Attack_Attack13* CValtan_BT_Attack_Attack13::Create(void* pArg)
{
	CValtan_BT_Attack_Attack13* pInstance = new CValtan_BT_Attack_Attack13;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack13::Free()
{
	__super::Free();
}
