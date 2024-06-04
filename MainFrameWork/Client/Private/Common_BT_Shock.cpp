#include "stdafx.h"
#include "Common_BT_Shock.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Shock::CCommon_BT_Shock()
{
}

void CCommon_BT_Shock::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Shock::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_52", 0, 0.2f);
		}

	}
	return BT_RUNNING;
}

void CCommon_BT_Shock::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Shock* CCommon_BT_Shock::Create(void* pArg)
{
	CCommon_BT_Shock* pInstance = new CCommon_BT_Shock;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Shock");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Shock::Free()
{
	__super::Free();
}
