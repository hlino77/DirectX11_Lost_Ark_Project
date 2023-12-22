#include "stdafx.h"
#include "Common_BT_Bound.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Bound::CCommon_BT_Bound()
{
}

void CCommon_BT_Bound::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Bound::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Bound::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Bound* CCommon_BT_Bound::Create(void* pArg)
{
	CCommon_BT_Bound* pInstance = new CCommon_BT_Bound;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Bound::Free()
{
	__super::Free();
}
