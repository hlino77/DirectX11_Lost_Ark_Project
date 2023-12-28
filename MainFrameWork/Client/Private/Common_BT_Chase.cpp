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

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[0].iAnimIndex))
		return BT_FAIL;

	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() <0.5f)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[1].iAnimIndex &&m_pGameObject->Get_ModelCom()->Get_NextAnim() != m_vecAnimDesc[1].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,
			m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame);
		return BT_RUNNING;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[0].iAnimIndex&& m_pGameObject->Get_ModelCom()->Get_NextAnim()	!= m_vecAnimDesc[0].iAnimIndex)
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[0].iAnimIndex, m_vecAnimDesc[0].fChangeTime,
			m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame);
	_float fSpeed = 1.5f * static_cast<CMonster*>(m_pGameObject)->Get_MoveSpeed();
	static_cast<CMonster*>(m_pGameObject)->Move_Dir(static_cast<CMonster*>(m_pGameObject)->Get_Target_Direction(), fSpeed, fTimeDelta);

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
