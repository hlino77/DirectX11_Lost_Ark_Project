#include "stdafx.h"
#include "Zombie_BT_Move.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Move::CZombie_BT_Move()
{
}

void CZombie_BT_Move::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Move::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Move::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CZombie_BT_Move* CZombie_BT_Move::Create(void* pArg)
{
	CZombie_BT_Move* pInstance = new CZombie_BT_Move;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Move");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Move::Free()
{
	__super::Free();
}
