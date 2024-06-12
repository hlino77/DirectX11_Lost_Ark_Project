#include "stdafx.h"
#include "Valtan_BT_Attack_CounterAttack.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "ColliderSphere.h"
#include "Effect.h"
#include "Effect_Manager.h"
#include "PartObject.h"
#include "Effect_Particle.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CValtan_BT_Attack_CounterAttack::CValtan_BT_Attack_CounterAttack()
{
}

void CValtan_BT_Attack_CounterAttack::OnStart()
{
	__super::OnStart();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[0].strAnimName, m_vecAnimDesc[0].fChangeTime, m_vecAnimDesc[0].iStartFrame, m_vecAnimDesc[0].iChangeFrame, m_vecAnimDesc[0].fAnimSpeed);
	static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.7f);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);

	m_Particles.clear();
	m_bParticle = false;
	m_bAttack = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_CounterAttack::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);

		Matrix matWorld = static_cast<CBoss_Valtan*>(m_pGameObject)->Get_Weapon()->Get_Part_WorldMatrix();
		for (auto& Particle : m_Particles)
		{
			Particle->Update_Pivot(matWorld);
		}

		if (m_bParticle == false)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START_OUTLIST(L"VTCounterAttackCharge", &tDesc, m_Particles);
			m_bParticle = true;
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex )
	{
		if (m_bAttack == false && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > 4)
		{
			static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(2.5f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(19.f);

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"ValtanCounterAttackTrail", &tDesc);
			m_bAttack = true;

			for (auto& Particle : m_Particles)
			{
				dynamic_cast<CEffect_Particle*>(Particle)->ParticleEnd();
			}
		}	
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 10 )
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	}
	if (m_vecAnimDesc[m_iCurrAnimation].bIsLoop && m_vecAnimDesc[m_iCurrAnimation].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && m_vecAnimDesc[m_iCurrAnimation].IsEndInstant)
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		else if (m_fLoopTime > m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime && !m_vecAnimDesc[m_iCurrAnimation].IsEndInstant && m_pGameObject->Get_ModelCom()->Is_AnimationEnd(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		{
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_fLoopTime = 0;
		}
		m_fLoopTime += fTimeDelta;
	}
	else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) - 3&& !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (m_iCurrAnimation == m_iMaxAnimation - 1)
			return BT_SUCCESS;
		else
		{
			m_iCurrAnimation++;

			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
			static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[m_iCurrAnimation].strAnimName, m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fAnimSpeed);
		}
	}
	Add_Sound_Channel(0, 0,L"Effect", L"Valtan#83 (499068403)");
	Add_Sound_Channel(0, 1,L"Effect", L"Valtan#84 (271442095)",  39);
	Add_Sound_Channel(0, 4,L"Effect", L"Valtan#88 (84610412)", 39);
	Add_Sound(2, 3,L"Effect", L"Valtan#85 (208205469)");
	Add_Sound(2, 2,L"Effect", L"Valtan#90 (338015342)");
	return BT_RUNNING;
}

void CValtan_BT_Attack_CounterAttack::OnEnd()
{
	__super::OnEnd();
	for (auto& Particle : m_Particles)
	{
		dynamic_cast<CEffect_Particle*>(Particle)->ParticleEnd();
	}
	CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Valtan#83 (499068403)");
	CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Valtan#84 (271442095)");
	CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Valtan#88 (84610412)");
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_CounterAttack* CValtan_BT_Attack_CounterAttack::Create(void* pArg)
{
	CValtan_BT_Attack_CounterAttack* pInstance = new CValtan_BT_Attack_CounterAttack;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_CounterAttack");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_CounterAttack::Free()
{
	__super::Free();
}
