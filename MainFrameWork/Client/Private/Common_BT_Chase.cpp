#include "stdafx.h"
#include "Common_BT_Chase.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"

CCommon_BT_Chase::CCommon_BT_Chase()
{
}

void CCommon_BT_Chase::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Chase::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() <0.9f)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction(fTimeDelta);
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimIndexFrame[1].first.iAnimIndex, m_vecAnimIndexFrame[1].first.fChangeTime,
			m_vecAnimIndexFrame[1].first.iStartFrame, m_vecAnimIndexFrame[1].first.iChangeFrame);
		return BT_RUNNING;
	}
	static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction(), 1.5f, fTimeDelta);

	return BT_RUNNING;
}

void CCommon_BT_Chase::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Chase* CCommon_BT_Chase::Create(void* pArg)
{
	CCommon_BT_Chase* pInstance = new CCommon_BT_Chase;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Chase::Free()
{
	__super::Free();
}
