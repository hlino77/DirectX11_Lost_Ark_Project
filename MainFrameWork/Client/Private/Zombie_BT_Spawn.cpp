#include "stdafx.h"
#include "Zombie_BT_Spawn.h"
#include "Monster.h"
#include "Model.h"
CZombie_BT_Spawn::CZombie_BT_Spawn()
{
}

void CZombie_BT_Spawn::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CZombie_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_FAIL;

	return BT_RUNNING;
}

void CZombie_BT_Spawn::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Spawn(false);
}

_bool CZombie_BT_Spawn::IsOut()
{
	if (static_cast<CMonster*>(m_pGameObject)->Is_Hit())
		return true;

	return false;
}

CZombie_BT_Spawn* CZombie_BT_Spawn::Create(void* pArg)
{
	CZombie_BT_Spawn* pInstance = new CZombie_BT_Spawn;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CZombie_BT_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CZombie_BT_Spawn::Free()
{
	__super::Free();
}
