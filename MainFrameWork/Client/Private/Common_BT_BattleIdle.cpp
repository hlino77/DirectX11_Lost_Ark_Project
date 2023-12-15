#include "stdafx.h"
#include "Common_BT_BattleIdle.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_BattleIdle::CCommon_BT_BattleIdle()
{
}

void CCommon_BT_BattleIdle::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_BattleIdle::OnUpdate(const _float& fTimeDelta)
{

	if( m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimIndexFrame[0].first.iAnimIndex))
		return BT_SUCCESS;
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction(fTimeDelta);
	return BT_RUNNING;
}

void CCommon_BT_BattleIdle::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_BattleIdle* CCommon_BT_BattleIdle::Create(void* pArg)
{
	CCommon_BT_BattleIdle* pInstance = new CCommon_BT_BattleIdle;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_BattleIdle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_BattleIdle::Free()
{
	__super::Free();
}
