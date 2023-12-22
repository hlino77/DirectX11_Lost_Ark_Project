#include "stdafx.h"
#include "Common_BT_TwistLand.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_TwistLand::CCommon_BT_TwistLand()
{
}

void CCommon_BT_TwistLand::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_TwistLand::OnUpdate(const _float& fTimeDelta)
{


	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_TwistLand::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_TwistLand* CCommon_BT_TwistLand::Create(void* pArg)
{
	CCommon_BT_TwistLand* pInstance = new CCommon_BT_TwistLand;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_TwistLand::Free()
{
	__super::Free();
}
