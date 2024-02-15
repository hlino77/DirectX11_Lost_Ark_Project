#include "stdafx.h"
#include "Common_BT_Stun.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Stun::CCommon_BT_Stun()
{
}

void CCommon_BT_Stun::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Stun::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_63", 0, 0.2f);
		}

	}

	return BT_RUNNING;
}

void CCommon_BT_Stun::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Stun* CCommon_BT_Stun::Create(void* pArg)
{
	CCommon_BT_Stun* pInstance = new CCommon_BT_Stun;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Stun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Stun::Free()
{
	__super::Free();
}
