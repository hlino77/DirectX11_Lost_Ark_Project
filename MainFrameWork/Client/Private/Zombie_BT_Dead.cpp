#include "stdafx.h"
#include "Zombie_BT_Dead.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Dead::CZombie_BT_Dead()
{
}

void CZombie_BT_Dead::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Dead::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Dead::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CZombie_BT_Dead* CZombie_BT_Dead::Create(void* pArg)
{
	CZombie_BT_Dead* pInstance = new CZombie_BT_Dead;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Dead::Free()
{
	__super::Free();
}
