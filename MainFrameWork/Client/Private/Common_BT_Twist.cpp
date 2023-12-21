#include "stdafx.h"
#include "Common_BT_Twist.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Twist::CCommon_BT_Twist()
{
}

void CCommon_BT_Twist::OnStart()
{
	__super::OnStart(0);

}

CBT_Node::BT_RETURN CCommon_BT_Twist::OnUpdate(const _float& fTimeDelta)
{


	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Twist::OnEnd()
{
	__super::OnEnd();

}



CCommon_BT_Twist* CCommon_BT_Twist::Create(void* pArg)
{
	CCommon_BT_Twist* pInstance = new CCommon_BT_Twist;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Twist::Free()
{
	__super::Free();
}
