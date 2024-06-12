#include "stdafx.h"
#include "Common_BT_Bug.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Bug::CCommon_BT_Bug()
{
}

void CCommon_BT_Bug::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Bug::OnUpdate(_float fTimeDelta)
{
	

	return BT_RUNNING;
}

void CCommon_BT_Bug::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Bug* CCommon_BT_Bug::Create(void* pArg)
{
	CCommon_BT_Bug* pInstance = new CCommon_BT_Bug;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Bug");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Bug::Free()
{
	__super::Free();
}
