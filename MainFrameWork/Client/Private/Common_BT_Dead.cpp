#include "stdafx.h"
#include "Common_BT_Dead.h"
#include "Monster.h"
#include "Model.h"
#include "GameInstance.h"
#include <Boss.h>

CCommon_BT_Dead::CCommon_BT_Dead()
{
}

void CCommon_BT_Dead::OnStart()
{
	__super::OnStart(0);
	static_cast<CMonster*>(m_pGameObject)->Deactivate_AllColliders();
}

CBT_Node::BT_RETURN CCommon_BT_Dead::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::BOSS)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::KING)
		{
			Add_Sound(0, 0, L"Effect", L"KING_74");
			Add_Sound(0, 1, L"Effect", L"KING_58");
		}
		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::GOLEM)
		{
			Add_Sound(0, 0, L"Effect", L"Rook_98");
		}
	}
	else if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Pawn_3"))
			{
				Add_Sound(0, 0, L"Effect", L"Pawn_3", 27, 0.2f);
			}			
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Pawn_63"))
			{
				Add_Sound(0, 1, L"Effect", L"Pawn_63", 0, 0.2f);
			}
		}

		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::GHOUL)
		{
			if (CSound_Manager::GetInstance()->Is_Channel_Playing(L"Ghoul_Dying"))
			{
				Add_Sound(0, 1, L"Effect", L"Ghoul_Dying", 0, 0.2f);
			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Dead::OnEnd()
{
	__super::OnEnd();
	static_cast<CMonster*>(m_pGameObject)->Set_Hit(false);
	static_cast<CMonster*>(m_pGameObject)->Set_Left(false);
	static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.f);
	static_cast<CMonster*>(m_pGameObject)->Set_Die();
}



CCommon_BT_Dead* CCommon_BT_Dead::Create(void* pArg)
{
	CCommon_BT_Dead* pInstance = new CCommon_BT_Dead;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Dead");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Dead::Free()
{
	__super::Free();
}
