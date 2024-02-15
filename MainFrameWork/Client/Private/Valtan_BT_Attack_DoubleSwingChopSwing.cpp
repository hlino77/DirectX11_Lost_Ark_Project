#include "stdafx.h"
#include "Valtan_BT_Attack_DoubleSwingChopSwing.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include <Skill_Valtan_DoughnutFirstHit.h>
#include <Skill_Valtan_DoughnutSecondHit.h>
#include "ColliderSphere.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"

CValtan_BT_Attack_DoubleSwingChopSwing::CValtan_BT_Attack_DoubleSwingChopSwing()
{
}

void CValtan_BT_Attack_DoubleSwingChopSwing::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_iEffectSequence = 0;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_DoubleSwingChopSwing::OnUpdate(const _float& fTimeDelta)
{
	_uint iAnimFrame0 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 49 && iAnimFrame0 < 53)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(1.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(15.f);
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 53 && iAnimFrame0 < 68)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);

	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 68 && iAnimFrame0 < 70)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);

	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 70 && iAnimFrame0 < 92)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);

	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 92 && iAnimFrame0 < 94)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);

	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 94)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);

	_uint iAnimFrame1 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && iAnimFrame1 >= 45 && iAnimFrame1 < 66)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);

	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && iAnimFrame1 >= 66)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);


	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && iAnimFrame0 >= 94 && m_bShoot[0])
	{
		Add_Sound(L"Effect", L"Valtan#103 (562236440)");
		m_bShoot[0] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 90.0f, 0.1f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutFirstHit", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.6f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(9.5f, 3.2f);
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#104 (160150074).wav");
			static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.5f);
			static_cast<CSkill*>(pSkill)->Set_LastTime(1.7f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && iAnimFrame1 >= 68 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutFirstHit", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#106 (917948767).wav");
			static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(7.f, 2.75f);
			static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.f);
			static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutSecondHit", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);		
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#107 (1071718533).wav");
			static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(10, 6);
			static_cast<CSkill*>(pSkill)->Set_BlinkTime(2.f);
			static_cast<CSkill*>(pSkill)->Set_LastTime(2.2f);
		}
	}
	{
		Add_Sound(0, 0,L"Effect", L"Boss_Voice");
		Add_Sound(0, 4,L"Effect", L"Valtan#84 (271442095)");		
		Add_Sound(0, 1,L"Effect", L"Valtan#101 (278609772)", 49);
		Add_Sound(0, 2,L"Effect", L"Valtan#102 (856646398)", 67);
		Add_Sound(1, 3,L"Effect", L"Valtan#105 (220501608)", 45);
	}
	Update_Effect();

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_DoubleSwingChopSwing::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}

void CValtan_BT_Attack_DoubleSwingChopSwing::Update_Effect()
{
	_uint iAnimFrame0 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex);
	_uint iAnimFrame1 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex);

	if (m_iEffectSequence == 0 && iAnimFrame0 >= 53)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Trail1", &tDesc);
		m_iEffectSequence = 1;
	}
	else if (m_iEffectSequence == 1 && iAnimFrame0 >= 69)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Trail2", &tDesc);
		m_iEffectSequence = 2;
	}
	else if (m_iEffectSequence == 2 && iAnimFrame0 >= 93)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Trail3", &tDesc);
		m_iEffectSequence = 3;
	}
	else if (m_iEffectSequence == 3 && iAnimFrame0 >= 95)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Chop", &tDesc);
		m_iEffectSequence = 4;
	}
	else if (m_iEffectSequence == 4 && iAnimFrame0 >= 102)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Warning1", &tDesc);
		EFFECT_START(L"VTDSCS_Ring1", &tDesc);
		m_iEffectSequence = 5;
	}

	else if (m_iEffectSequence == 5 && iAnimFrame1 >= 49)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Trail4", &tDesc);
		m_iEffectSequence = 6;
	}

	else if (m_iEffectSequence == 6 && iAnimFrame1 >= 56)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTDSCS_Warning2", &tDesc);
		EFFECT_START(L"VTDSCS_Warning3", &tDesc);
		EFFECT_START(L"VTDSCS_Ring2", &tDesc);
		EFFECT_START(L"VTDSCS_Ring3", &tDesc);
		m_iEffectSequence = 7;
	}
}



CValtan_BT_Attack_DoubleSwingChopSwing* CValtan_BT_Attack_DoubleSwingChopSwing::Create(void* pArg)
{
	CValtan_BT_Attack_DoubleSwingChopSwing* pInstance = new CValtan_BT_Attack_DoubleSwingChopSwing;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_DoubleSwingChopSwing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_DoubleSwingChopSwing::Free()
{
	__super::Free();
}
