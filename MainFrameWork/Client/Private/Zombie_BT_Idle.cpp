#include "stdafx.h"
#include "Zombie_BT_Idle.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Idle::CZombie_BT_Idle()
{
}

void CZombie_BT_Idle::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Idle::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	if (static_cast<CMonster*>(m_pGameObject)->Get_Target_Distance() < 10.f && !static_cast<CMonster*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CZombie_BT_Idle::OnEnd()
{
	__super::OnEnd();
}



CZombie_BT_Idle* CZombie_BT_Idle::Create(void* pArg)
{
	CZombie_BT_Idle* pInstance = new CZombie_BT_Idle;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Idle::Free()
{
	__super::Free();
}
