#include "stdafx.h"
#include "Valtan_BT_Attack_RainingAxe.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"
#include <Skill.h>	
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "SKill_Valtan_RainingAxe.h"



CValtan_BT_Attack_RainingAxe::CValtan_BT_Attack_RainingAxe()
{
}

void CValtan_BT_Attack_RainingAxe::OnStart()
{
	__super::OnStart();
	if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
		m_iAttackCount = 9;
	else
		m_iAttackCount = 3;
	m_fAttackTerm = 0.f;
	m_bShoot = true;

	m_bWarning = false;
	
}

CBT_Node::BT_RETURN CValtan_BT_Attack_RainingAxe::OnUpdate(_float fTimeDelta)
{

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
	{
		m_pGameObject->Set_Render(false);
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2 && m_fLoopTime < m_vecAnimDesc[1].fMaxLoopTime-1.f&& !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_pGameObject->Set_Render(false);
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, static_cast<CBoss*>(m_pGameObject)->Get_NearTarget_Position());
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2 && m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime - 1.f && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (m_bWarning == false)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_RainAxe2WarningIn", &tDesc);

			m_bWarning = true;
		}
	}


	if (m_iCurrAnimation == 2)
		m_pGameObject->Set_Render(true);
	
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_fAttackTerm += fTimeDelta;

		if (m_fAttackTerm > 1.f && m_iAttackCount > 0)
		{
			m_iAttackCount--;
			m_fAttackTerm = 0.f;
			if (m_iAttackCount == 0)
			{
				m_fLoopTime = m_vecAnimDesc[1].fMaxLoopTime - 2.f;
			}

			if (m_iAttackCount == 2 && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
			{
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
				EFFECT_START(L"VTAxeRainEndWarning", &tDesc);
			}
			if (CGameInstance::GetInstance()->Random_Coin(0.5f))
				Add_Sound( L"Effect", L"Valtan#117 (516386150)");
			else
				Add_Sound(L"Effect", L"Valtan#118 (282820102)");
			vector<CGameObject*> vecTargets = CGameInstance::GetInstance()->Find_GameObjects(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER);
			if (!vecTargets.empty())
				for (auto& Object : vecTargets)
				{
					CSkill::ModelDesc ModelDesc = {};
					ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
					ModelDesc.iObjectID = -1;
					ModelDesc.pOwner = m_pGameObject;

					CSKill_Valtan_RainingAxe::RainAxeDesc tDesc;
					tDesc.tSkillDesc = ModelDesc;
					tDesc.vTargetPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

					CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_RainingAxe", &tDesc);
					if (CGameInstance::GetInstance()->Random_Coin(0.5f))
						static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#121 (557856096).wav");
					else
						static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#122 (225164104).wav");
				}
			vecTargets = CGameInstance::GetInstance()->Find_GameObjects(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER);
			if (!vecTargets.empty())
				for (auto& Object : vecTargets)
				{
					CSkill::ModelDesc ModelDesc = {};
					ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
					ModelDesc.iObjectID = -1;
					ModelDesc.pOwner = m_pGameObject;

					CSKill_Valtan_RainingAxe::RainAxeDesc tDesc;
					tDesc.tSkillDesc = ModelDesc;
					tDesc.vTargetPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

					CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_RainingAxe", &tDesc);
				}
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 8 && m_bShoot)
	{
		m_bShoot = false;	
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.8f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(11.f);
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VTAxeRainEnd1", &tDesc);
		}
		else
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(5.8f);
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
				static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#107 (1071718533).wav");
				static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(30.f, 5.0f);
				static_cast<CSkill*>(pSkill)->Set_BlinkTime(2.f);
				static_cast<CSkill*>(pSkill)->Set_LastTime(2.2f);
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_RainAxeIn", &tDesc);
			EFFECT_START(L"VT_RainAxeOut", &tDesc);
			EFFECT_START(L"VT_RainAxe2WarningOut", &tDesc);
		}
	}

	//Sound
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#113 (872104708)");
		Add_Sound(0, 1, L"Effect", L"Valtan#153 (153317365)", 24);
		Add_Sound(2, 2, L"Effect", L"Valtan#43 (1015057785).wav", 7);
		Add_Sound(2, 3, L"Effect", L"Valtan#42 (236377491)", 7);
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_RainingAxe::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_RainingAxe* CValtan_BT_Attack_RainingAxe::Create(void* pArg)
{
	CValtan_BT_Attack_RainingAxe* pInstance = new CValtan_BT_Attack_RainingAxe;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_RainingAxe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_RainingAxe::Free()
{
	__super::Free();
}
