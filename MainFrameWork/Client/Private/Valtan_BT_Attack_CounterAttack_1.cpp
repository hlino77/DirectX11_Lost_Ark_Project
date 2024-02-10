#include "stdafx.h"
#include "Valtan_BT_Attack_CounterAttack_1.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "ColliderSphere.h"
#include "Effect.h"
#include "Effect_Manager.h"

CValtan_BT_Attack_CounterAttack_1::CValtan_BT_Attack_CounterAttack_1()
{
}

void CValtan_BT_Attack_CounterAttack_1::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_9_01_end", 0.2f, 0, 0, 1.15f);

	m_bAttack = false;
	m_bEffectLast = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_CounterAttack_1::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex)
	{
		if (m_bAttack == false)
		{
			static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(5.f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(45.f);

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VTCounterSuccess", &tDesc);

			m_bAttack = true;
		}

		if (m_bEffectLast == false && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 12)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"ValtanCounterAttackTrail", &tDesc);
			m_bEffectLast = true;
		}
		
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 21)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_CounterAttack_1::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_CounterAttack_1* CValtan_BT_Attack_CounterAttack_1::Create(void* pArg)
{
	CValtan_BT_Attack_CounterAttack_1* pInstance = new CValtan_BT_Attack_CounterAttack_1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_CounterAttack_1");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_CounterAttack_1::Free()
{
	__super::Free();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}
