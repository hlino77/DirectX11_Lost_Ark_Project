#include "stdafx.h"
#include "Valtan_BT_Attack_FistSmashExplosion.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect.h"


CValtan_BT_Attack_FistSmashExplosion::CValtan_BT_Attack_FistSmashExplosion()
{
}

void CValtan_BT_Attack_FistSmashExplosion::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
	m_bWarning = false;
	m_bLastAttack = false;
	m_fLastAttackDelay = 0.5f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_FistSmashExplosion::OnUpdate(const _float& fTimeDelta)
{
	if (m_bWarning == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTFistSmashWarning", &tDesc);
		m_bWarning = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 31&& m_bShoot)
	{
		m_bShoot = false;
		Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#106 (917948767)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.2f, 100.0f, 0.5f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;


		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_SphereRadius(5.5f);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutTerm2sec", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);		
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#112 (934806095).wav");
			static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(12.5f, 3.5f);
		}


		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VTFistSmash", &tDesc);
		EFFECT_START(L"VTFistSmashWarning2", &tDesc);
		EFFECT_START(L"VTFistSmash2", &tDesc);
		m_bWarning = true;


	}

	if (m_bLastAttack == false && m_bShoot == false)
	{
		m_fLastAttackDelay -= fTimeDelta;
		if (m_fLastAttackDelay <= 0.0f)
		{
			CSkill::ModelDesc ModelDesc = {};
			ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
			ModelDesc.iObjectID = -1;
			ModelDesc.pOwner = m_pGameObject;

			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereTerm", &ModelDesc);
			if (pSkill != nullptr && m_pGameObject->Get_NearTarget() != nullptr)
			{
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &m_pGameObject->Get_NearTarget()->Get_TransformCom()->Get_WorldMatrix();
				EFFECT_START(L"VTFistSmash3", &tDesc);

				Vec3 vPos = m_pGameObject->Get_NearTarget()->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_NearTarget()->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(3.f);
				static_cast<CSkill*>(pSkill)->Set_BlinkTime(3.4f);
				static_cast<CSkill*>(pSkill)->Set_LastTime(3.6f);
				static_cast<CSkill*>(pSkill)->Set_SoundTag(L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#37 (464077464).wav");
			}

			m_bLastAttack = true;
		}
	}
	Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#307 (599260869)");

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_FistSmashExplosion::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_FistSmashExplosion* CValtan_BT_Attack_FistSmashExplosion::Create(void* pArg)
{
	CValtan_BT_Attack_FistSmashExplosion* pInstance = new CValtan_BT_Attack_FistSmashExplosion;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FistSmashExplosionValtan_BT_Attack_FistSmashExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_FistSmashExplosion::Free()
{
	__super::Free();
}
