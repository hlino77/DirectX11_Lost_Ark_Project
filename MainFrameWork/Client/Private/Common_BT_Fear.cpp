#include "stdafx.h"
#include "Common_BT_Fear.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Fear::CCommon_BT_Fear()
{
}

void CCommon_BT_Fear::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Fear::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_58");
		}

	}
	

	return BT_RUNNING;
}

void CCommon_BT_Fear::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Fear* CCommon_BT_Fear::Create(void* pArg)
{
	CCommon_BT_Fear* pInstance = new CCommon_BT_Fear;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Fear");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Fear::Free()
{
	__super::Free();
}
