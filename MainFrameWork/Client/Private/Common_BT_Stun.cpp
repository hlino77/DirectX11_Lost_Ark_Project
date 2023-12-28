#include "stdafx.h"
#include "Common_BT_Stun.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Stun::CCommon_BT_Stun()
{
}

void CCommon_BT_Stun::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Stun::OnUpdate(const _float& fTimeDelta)
{


	return BT_RUNNING;
}

void CCommon_BT_Stun::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Stun* CCommon_BT_Stun::Create(void* pArg)
{
	CCommon_BT_Stun* pInstance = new CCommon_BT_Stun;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Stun::Free()
{
	__super::Free();
}
