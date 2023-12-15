#include "stdafx.h"
#include "Common_BT_Idle.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Idle::CCommon_BT_Idle()
{
}

void CCommon_BT_Idle::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Idle::OnUpdate(const _float& fTimeDelta)
{
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CCommon_BT_Idle::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Idle* CCommon_BT_Idle::Create(void* pArg)
{
	CCommon_BT_Idle* pInstance = new CCommon_BT_Idle;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Idle::Free()
{
	__super::Free();
}
