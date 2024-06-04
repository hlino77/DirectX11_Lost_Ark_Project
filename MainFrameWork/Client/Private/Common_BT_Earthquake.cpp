#include "stdafx.h"
#include "Common_BT_Earthquake.h"
#include "Monster.h"
#include "Model.h"
CCommon_BT_Earthquake::CCommon_BT_Earthquake()
{
}

void CCommon_BT_Earthquake::OnStart()
{
	__super::OnStart(0);
}

CBT_Node::BT_RETURN CCommon_BT_Earthquake::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_53", 0, 0.2f);
		}

	}
	return BT_RUNNING;
}

void CCommon_BT_Earthquake::OnEnd()
{
	__super::OnEnd();
}



CCommon_BT_Earthquake* CCommon_BT_Earthquake::Create(void* pArg)
{
	CCommon_BT_Earthquake* pInstance = new CCommon_BT_Earthquake;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Earthquake");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Earthquake::Free()
{
	__super::Free();
}
