#include "stdafx.h"
#include "Common_BT_Spawn.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Spawn::CCommon_BT_Spawn()
{
}

void CCommon_BT_Spawn::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->	LookAt_Target_Direction();
}

CBT_Node::BT_RETURN CCommon_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{

	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimIndexFrame[0].first.iAnimIndex) == m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimIndexFrame[0].first.iAnimIndex) - 3)
		return BT_FAIL;

	return BT_RUNNING;
}

void CCommon_BT_Spawn::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Spawn(false);
}



CCommon_BT_Spawn* CCommon_BT_Spawn::Create(void* pArg)
{
	CCommon_BT_Spawn* pInstance = new CCommon_BT_Spawn;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Spawn");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Spawn::Free()
{
	__super::Free();
}
