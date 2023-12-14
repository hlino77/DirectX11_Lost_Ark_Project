#include "stdafx.h"
#include "Zombie_BT_BattleIdle.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_BattleIdle::CZombie_BT_BattleIdle()
{
}

void CZombie_BT_BattleIdle::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_BattleIdle::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CZombie_BT_BattleIdle::OnEnd()
{
	__super::OnEnd();
}



CZombie_BT_BattleIdle* CZombie_BT_BattleIdle::Create(void* pArg)
{
	CZombie_BT_BattleIdle* pInstance = new CZombie_BT_BattleIdle;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_BattleIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_BattleIdle::Free()
{
	__super::Free();
}
