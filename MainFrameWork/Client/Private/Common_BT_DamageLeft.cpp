#include "stdafx.h"
#include "Common_BT_DamageLeft.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_DamageLeft::CCommon_BT_DamageLeft()
{
}

void CCommon_BT_DamageLeft::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_DamageLeft::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_DamageLeft::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_DamageLeft* CCommon_BT_DamageLeft::Create(void* pArg)
{
	CCommon_BT_DamageLeft* pInstance = new CCommon_BT_DamageLeft;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Damage");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_DamageLeft::Free()
{
	__super::Free();
}
