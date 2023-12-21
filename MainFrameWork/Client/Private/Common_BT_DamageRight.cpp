#include "stdafx.h"
#include "Common_BT_DamageRight.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_DamageRight::CCommon_BT_DamageRight()
{
}

void CCommon_BT_DamageRight::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_DamageRight::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_DamageRight::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_DamageRight* CCommon_BT_DamageRight::Create(void* pArg)
{
	CCommon_BT_DamageRight* pInstance = new CCommon_BT_DamageRight;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_DamageRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_DamageRight::Free()
{
	__super::Free();
}
