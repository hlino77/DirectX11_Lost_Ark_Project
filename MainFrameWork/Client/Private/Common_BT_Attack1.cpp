#include "stdafx.h"
#include "Common_BT_Attack1.h"
#include "Model.h"
#include "ColliderSphere.h"
#include <Boss.h>

CCommon_BT_Attack1::CCommon_BT_Attack1()
{
}

void CCommon_BT_Attack1::OnStart()
{
	__super::OnStart(0);

	m_iMaxSound = 20;
	for (size_t i = 0; i < m_iMaxSound; i++)
	{
		m_bSoundOn[i] = false;
	}
}

CBT_Node::BT_RETURN CCommon_BT_Attack1::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ObjectType() ==OBJ_TYPE::MONSTER)
	{
		if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackStartFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, true);
		if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackEndFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
	}
	else if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::BOSS)
	{
		if (!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackStartFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		{
			dynamic_cast<CBoss*>(m_pGameObject)->Set_Atk(dynamic_cast<CBoss*>(m_pGameObject)->Get_BaseAtk());
			dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(	dynamic_cast<CBoss*>(m_pGameObject)->Get_BaseForce());
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
		}
		if (m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive() && dynamic_cast<CMonster*>(m_pGameObject)->Get_BasicAttackEndFrame() <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
			dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);

	}
	if (m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta * 5.f);

	if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::BOSS)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Get_BossType() == CBoss::GOLEM)
		{
			Add_Sound(0, 0, TEXT("Effect"), TEXT("Rook_1"), 10, 0.5f);
			Add_Sound(0, 1, TEXT("Effect"), TEXT("Rook_40"), 12, 0.9f);
		}
	}	
	else if (m_pGameObject->Get_ObjectType() == OBJ_TYPE::MONSTER)
	{
		if (static_cast<CMonster*>(m_pGameObject)->Get_MonsterType() == CMonster::PAWN)
		{
			Add_Sound(0, 0, L"Effect", L"Pawn_32", 15, 0.2f);
			Add_Sound(0, 1, L"Effect", L"Pawn_77", 15, 0.2f);
			Add_Sound(0, 2, L"Effect", L"Pawn_25", 25, 0.2f);
			Add_Sound(0, 3, L"Effect", L"Pawn_80", 25, 0.2f);
		}
	}

	return __super::OnUpdate(fTimeDelta);
}

void CCommon_BT_Attack1::OnEnd()
{
	__super::OnEnd();
	dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_MONSTER, false);
}



CCommon_BT_Attack1* CCommon_BT_Attack1::Create(void* pArg)
{
	CCommon_BT_Attack1* pInstance = new CCommon_BT_Attack1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CCommon_BT_Attack1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCommon_BT_Attack1::Free()
{
	__super::Free();
}
