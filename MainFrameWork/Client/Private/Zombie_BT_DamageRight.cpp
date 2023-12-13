#include "stdafx.h"
#include "Zombie_BT_DamageRight.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_DamageRight::CZombie_BT_DamageRight()
{
}

void CZombie_BT_DamageRight::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_DamageRight::OnUpdate(const _float& fTimeDelta)
{
	
	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_DamageRight::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);

}



CZombie_BT_DamageRight* CZombie_BT_DamageRight::Create(void* pArg)
{
	CZombie_BT_DamageRight* pInstance = new CZombie_BT_DamageRight;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_DamageRight");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_DamageRight::Free()
{
	__super::Free();
}
