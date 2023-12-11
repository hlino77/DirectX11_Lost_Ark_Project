#include "stdafx.h"
#include "Zombie_BT_Maz.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Maz::CZombie_BT_Maz()
{
}

void CZombie_BT_Maz::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Maz::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Maz::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CZombie_BT_Maz* CZombie_BT_Maz::Create(void* pArg)
{
	CZombie_BT_Maz* pInstance = new CZombie_BT_Maz;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Maz");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Maz::Free()
{
	__super::Free();
}
