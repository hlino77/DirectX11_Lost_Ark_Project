#include "stdafx.h"
#include "Valtan_BT_Attack_WhirlWind.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include "Effect_Manager.h"
#include "Effect.h"

CValtan_BT_Attack_WhirlWind::CValtan_BT_Attack_WhirlWind()
{
}

void CValtan_BT_Attack_WhirlWind::OnStart()
{
	__super::OnStart();

	m_pEffect = nullptr;
	m_bAttack = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_WhirlWind::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		if (m_bAttack == false)
		{
			vector<CEffect*> Effects;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"ValtanWhirlWindTrail", &tDesc, Effects);
			m_pEffect = Effects.front();

			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(4.5f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(45.f);
			m_bAttack = true;
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 12)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	}
	{
		Add_Sound(0, 0,L"Effect",  L"Valtan#88 (84610412)");
		Add_Sound(1, 1,L"Effect",  L"Valtan#222 (193078861)");
		Add_Sound(2, 2,L"Effect",  L"Valtan#223 (403055014)");
		Add_Sound(2, 3, L"Effect", L"Valtan#113 (872104708)");
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_WhirlWind::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_WhirlWind* CValtan_BT_Attack_WhirlWind::Create(void* pArg)
{
	CValtan_BT_Attack_WhirlWind* pInstance = new CValtan_BT_Attack_WhirlWind;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_WhirlWind");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_WhirlWind::Free()
{
	__super::Free();
}
