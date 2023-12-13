#include "stdafx.h"
#include "Zombie_BT_Attack2.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Attack2::CZombie_BT_Attack2()
{
}

void CZombie_BT_Attack2::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Attack2::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CZombie_BT_Attack2::OnEnd()
{
	__super::OnEnd();
}



CZombie_BT_Attack2* CZombie_BT_Attack2::Create(void* pArg)
{
	CZombie_BT_Attack2* pInstance = new CZombie_BT_Attack2;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Attack2");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Attack2::Free()
{
	__super::Free();
}
