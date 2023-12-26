#include "stdafx.h"
#include "Common_BT_Dead.h"
#include "Monster.h"
#include "Model.h"

CCommon_BT_Dead::CCommon_BT_Dead()
{
}

void CCommon_BT_Dead::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Deactivate_AllColliders();
}

CBT_Node::BT_RETURN CCommon_BT_Dead::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CCommon_BT_Dead::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CCommon_BT_Dead* CCommon_BT_Dead::Create(void* pArg)
{
	CCommon_BT_Dead* pInstance = new CCommon_BT_Dead;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Dead::Free()
{
	__super::Free();
}
