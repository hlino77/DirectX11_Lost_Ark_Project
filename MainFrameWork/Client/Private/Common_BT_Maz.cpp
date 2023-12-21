#include "stdafx.h"
#include "Common_BT_Maz.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Maz::CCommon_BT_Maz()
{
}

void CCommon_BT_Maz::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Maz::OnUpdate(const _float& fTimeDelta)
{
	
	

	return OnUpdate(fTimeDelta);
}

void CCommon_BT_Maz::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Maz* CCommon_BT_Maz::Create(void* pArg)
{
	CCommon_BT_Maz* pInstance = new CCommon_BT_Maz;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Maz");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Maz::Free()
{
	__super::Free();
}
