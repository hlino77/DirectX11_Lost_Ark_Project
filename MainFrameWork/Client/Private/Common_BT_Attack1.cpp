#include "stdafx.h"
#include "Common_BT_Attack1.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Attack1::CCommon_BT_Attack1()
{
}

void CCommon_BT_Attack1::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Attack1::OnUpdate(const _float& fTimeDelta)
{

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Attack1::OnEnd()
{
	__super::OnEnd();

}



CCommon_BT_Attack1* CCommon_BT_Attack1::Create(void* pArg)
{
	CCommon_BT_Attack1* pInstance = new CCommon_BT_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Attack1::Free()
{
	__super::Free();
}
