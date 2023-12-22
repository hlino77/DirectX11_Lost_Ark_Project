#include "stdafx.h"
#include "Common_BT_Damage2.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Damage2::CCommon_BT_Damage2()
{
}

void CCommon_BT_Damage2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Damage2::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_Damage2::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CCommon_BT_Damage2* CCommon_BT_Damage2::Create(void* pArg)
{
	CCommon_BT_Damage2* pInstance = new CCommon_BT_Damage2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Damage2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Damage2::Free()
{
	__super::Free();
}
