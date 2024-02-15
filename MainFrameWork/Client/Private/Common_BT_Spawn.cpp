#include "stdafx.h"
#include "Common_BT_Spawn.h"
#include "Monster.h"
#include "Model.h"
#include <Boss.h>
CCommon_BT_Spawn::CCommon_BT_Spawn()
{
}

void CCommon_BT_Spawn::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction();
}

CBT_Node::BT_RETURN CCommon_BT_Spawn::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::BOSS)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::KING)
		{
			Add_Sound(0, 0, L"Effect", L"KING_75");
			Add_Sound(0, 1, L"Effect", L"KING_78",104);
		}
		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::GOLEM)
		{
			Add_Sound(0, 0, L"Effect", L"Rook_95");
		}
	}
	else if(m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER) 
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Pawn_51"))
			{
				Add_Sound_Channel(0, 0, L"Effect", L"Pawn_51", 0, 0.2f);
				Add_Sound_Channel(0, 1, L"Effect", L"Pawn_2", 0, 0.2f);
			}
		}
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::GHOUL)
		{
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Ghoul_Spawn"))
			{
				Add_Sound(0, 0, L"Effect", L"Ghoul_Spawn", 0, 0.2f);
			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
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
