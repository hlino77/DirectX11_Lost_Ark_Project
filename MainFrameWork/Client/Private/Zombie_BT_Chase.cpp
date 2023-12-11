#include "stdafx.h"
#include "Zombie_BT_Chase.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Chase::CZombie_BT_Chase()
{
}

void CZombie_BT_Chase::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Chase::OnUpdate(const _float& fTimeDelta)
{

	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Chase::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CZombie_BT_Chase* CZombie_BT_Chase::Create(void* pArg)
{
	CZombie_BT_Chase* pInstance = new CZombie_BT_Chase;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Chase");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Chase::Free()
{
	__super::Free();
}