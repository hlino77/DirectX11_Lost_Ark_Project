#include "stdafx.h"
#include "Common_BT_BoundLand.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_BoundLand::CCommon_BT_BoundLand()
{
}

void CCommon_BT_BoundLand::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_BoundLand::OnUpdate(const _float& fTimeDelta)
{


	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_BoundLand::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_BoundLand* CCommon_BT_BoundLand::Create(void* pArg)
{
	CCommon_BT_BoundLand* pInstance = new CCommon_BT_BoundLand;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_BoundLand::Free()
{
	__super::Free();
}
