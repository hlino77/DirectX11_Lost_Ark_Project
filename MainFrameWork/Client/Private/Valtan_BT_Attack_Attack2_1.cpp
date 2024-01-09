#include "stdafx.h"
#include "Valtan_BT_Attack_Attack2_1.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"
#include <Boss.h>

CValtan_BT_Attack_Attack2_1::CValtan_BT_Attack_Attack2_1()
{
}

void CValtan_BT_Attack_Attack2_1::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack2_1::OnUpdate(const _float& fTimeDelta)
{
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack2_1::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack2_1* CValtan_BT_Attack_Attack2_1::Create(void* pArg)
{
	CValtan_BT_Attack_Attack2_1* pInstance = new CValtan_BT_Attack_Attack2_1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack2_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack2_1::Free()
{
	__super::Free();
}
