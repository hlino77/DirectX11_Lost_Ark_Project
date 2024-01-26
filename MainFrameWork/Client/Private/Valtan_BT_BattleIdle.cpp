#include "stdafx.h"
#include "Valtan_BT_BattleIdle.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
CValtan_BT_BattleIdle::CValtan_BT_BattleIdle()
{
}

void CValtan_BT_BattleIdle::OnStart()
{
	__super::OnStart(0);
	m_fIdletime = 0.f;
}

CBT_Node::BT_RETURN CValtan_BT_BattleIdle::OnUpdate(const _float& fTimeDelta)
{
	if (static_cast<CMonster*>(m_pGameObject)->Is_Hit())
		return BT_FAIL;
	m_pGameObject->Get_TransformCom()->Turn_Speed(Vec3(0.f,1.f,0.f), XMConvertToRadians(180.f),fTimeDelta);

	m_fIdletime += fTimeDelta;
	if (m_fIdletime> 1.f)
		return BT_SUCCESS;

	return BT_RUNNING;
}

void CValtan_BT_BattleIdle::OnEnd()
{
	__super::OnEnd();
	m_fIdletime = 0.f;
}



CValtan_BT_BattleIdle* CValtan_BT_BattleIdle::Create(void* pArg)
{
	CValtan_BT_BattleIdle* pInstance = new CValtan_BT_BattleIdle;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_BattleIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_BattleIdle::Free()
{
	__super::Free();
}
