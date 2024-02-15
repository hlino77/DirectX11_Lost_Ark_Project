#include "stdafx.h"
#include "Valtan_BT_Attack_SilenceChop.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect.h"


CValtan_BT_Attack_SilenceChop::CValtan_BT_Attack_SilenceChop()
{
}

void CValtan_BT_Attack_SilenceChop::OnStart()
{
	__super::OnStart();
	m_bOutSide = m_pGameObject->Get_TargetPos().x;
	m_bShoot[0] = true;
	m_bShoot[1] = true;

	m_bWarning = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_SilenceChop::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 23 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.2f, 0.7f);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.2f, 0.7f);
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SilenceSphere", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_StatusEffectDuration(7.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_Silence", &tDesc);
	}

	if (m_iCurrAnimation == 2)
	{
		if (m_bOutSide == true)
		{
			static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.05f, 0.7f);
		}

		if (m_bWarning == false && m_fLoopTime >= 0.8f)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

			if (m_bOutSide == true)
			{
				EFFECT_START(L"VT_SilenceWarning2", &tDesc);
			}
			else
			{
				EFFECT_START(L"VT_SilenceWarning1", &tDesc);
			}

			m_bWarning = true;
		}
	}




	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 7 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 110.0f, 0.5f, 10.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Add_Sound(L"Effect", L"Valtan#365 (21085455)");
		if (m_bOutSide)
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				vPos += vLook * 3.f;
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(30.f, 5.0f);
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
				EFFECT_START(L"VT_SilenceOut", &tDesc);
			}
		}
		else
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				vPos += vLook * 3.f;
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				static_cast<CSkill*>(pSkill)->Set_SphereRadius(5.8f);
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
				EFFECT_START(L"VT_SilenceIn", &tDesc);
			}
		}
		vector<CGameObject*> vecTargets = CGameInstance::GetInstance()->Find_GameObjects(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER);
		if (!vecTargets.empty())
			for (auto& Object : vecTargets)
			{
				CSkill::ModelDesc ModelDesc = {};
				ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
				ModelDesc.iObjectID = -1;
				ModelDesc.pOwner = m_pGameObject;


				CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereTerm", &ModelDesc);
				if (pSkill != nullptr)
				{
					Vec3 vPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					Vec3 vLook = Object->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
					vLook.Normalize();
					pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
					pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					static_cast<CSkill*>(pSkill)->Set_Atk(30);
					static_cast<CSkill*>(pSkill)->Set_Force(32.f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.0f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);

					CEffect_Manager::EFFECTPIVOTDESC tDesc;
					tDesc.pPivotMatrix = &Object->Get_TransformCom()->Get_WorldMatrix();
					EFFECT_START(L"VT_SilencePlayer", &tDesc);

					static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#249 (953298922).wav");
				}
			}
		vecTargets = CGameInstance::GetInstance()->Find_GameObjects(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER);
		if (!vecTargets.empty())
			for (auto& Object : vecTargets)
			{
				CSkill::ModelDesc ModelDesc = {};
				ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
				ModelDesc.iObjectID = -1;
				ModelDesc.pOwner = m_pGameObject;


				CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereTerm", &ModelDesc);
				if (pSkill != nullptr)
				{
					Vec3 vPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					Vec3 vLook = Object->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
					vLook.Normalize();
					pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
					static_cast<CSkill*>(pSkill)->Set_Atk(30);
					static_cast<CSkill*>(pSkill)->Set_Force(32.f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.0f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);

					CEffect_Manager::EFFECTPIVOTDESC tDesc;
					tDesc.pPivotMatrix = &Object->Get_TransformCom()->Get_WorldMatrix();
					EFFECT_START(L"VT_SilencePlayer", &tDesc);

					static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#249 (953298922).wav");

				}
			}
	}

	//sound
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#78 (647906992)",18);
		Add_Sound(0, 1, L"Effect", L"Valtan#135 (936435461)", 18);
		Add_Sound(1, 2, L"Effect", L"Valtan#302 (748487226)");
	}
	return __super::OnUpdate(fTimeDelta);
}


void CValtan_BT_Attack_SilenceChop::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_SilenceChop* CValtan_BT_Attack_SilenceChop::Create(void* pArg)
{
	CValtan_BT_Attack_SilenceChop* pInstance = new CValtan_BT_Attack_SilenceChop;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_SilenceChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_SilenceChop::Free()
{
	__super::Free();
}
