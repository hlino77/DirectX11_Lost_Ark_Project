#include "stdafx.h"
#include "Valtan_BT_Attack_BugSmash.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"
#include <Skill.h>	
#include "NavigationMgr.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Sound_Manager.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Effect_Particle.h"
#include "Effect_Trail.h"

CValtan_BT_Attack_BugSmash::CValtan_BT_Attack_BugSmash()
{
}

void CValtan_BT_Attack_BugSmash::OnStart()
{
	__super::OnStart();
	m_bShoot[0] =true;
	m_bShoot[1] = true;
	m_SmokeEffects.clear();
	m_ChargeEffects.clear();

	m_fChargeTrailAcc = 0.0f;
	m_fChargeTrailTime = 0.05f;

	m_pRushWarning = nullptr;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_BugSmash::OnUpdate(const _float& fTimeDelta)
{
	m_pGameObject->Get_TransformCom()->Set_Up(Vec3(0.f,1.f,0.f));
	if (m_SmokeEffects.empty() == false)
	{
		Matrix matWorld = Get_GrabMatrix();

		for (auto& Effect : m_SmokeEffects)
		{
			Effect->Update_Pivot(matWorld);
		}
	}

	Update_ChargeTrail(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex)
	{
		if (m_SmokeEffects.empty())
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &Get_GrabMatrix();
			EFFECT_START_OUTLIST(L"VT_BugSmoke", &tDesc, m_SmokeEffects);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		m_pGameObject->Set_Render(false);
	}
	if (m_iCurrAnimation == 2)
		m_pGameObject->Set_Render(true);
	if ( m_iCurrAnimation == 6 || m_iCurrAnimation == 5&& m_fLoopTime > 1.f)
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.6f);

	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex && m_fLoopTime <= 1.0f && m_pGameObject->Get_ModelCom()->IsNext() == false)
	{
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);	
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(vLook);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex&& !m_pGameObject->Get_ModelCom()->IsNext() && m_fLoopTime > 1.f && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.y = 0.f;
		vLook.Normalize();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(vLook);
		if (m_pRushWarning == nullptr)
		{
			vector<CEffect*> Effects;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"VT_BugRushWarning", &tDesc, Effects);
			m_pRushWarning = Effects.front();
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
	{
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[14].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation = 10;
		m_bShoot[1] = true;
		Reset_Sound();
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		m_fLoopTime = 0;
		return BT_RUNNING;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 43 && m_bShoot[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_BugStart", &tDesc);

		m_bShoot[0] = false;	
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Valtan#274 (200670410).wav", 1.f);
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Valtan#139 (195687587).wav", 1.f);
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.2f, 30.0f, 0.1f, 10.0f);
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
					static_cast<CSkill*>(pSkill)->Set_Force(42.f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.0f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);
					static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#249 (953298922).wav");

					CEffect_Manager::EFFECTPIVOTDESC tDesc;
					tDesc.pPivotMatrix = &Object->Get_TransformCom()->Get_WorldMatrix();
					EFFECT_START(L"VT_BugStart2", &tDesc);
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
					static_cast<CSkill*>(pSkill)->Set_Force(42.f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.0f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);
					static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#249 (953298922).wav");

					CEffect_Manager::EFFECTPIVOTDESC tDesc;
					tDesc.pPivotMatrix = &Object->Get_TransformCom()->Get_WorldMatrix();
					EFFECT_START(L"VT_BugStart2", &tDesc);
				}
			}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[8].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		if (m_pRushWarning != nullptr)
		{
			m_pRushWarning->EffectEnd();
			m_pRushWarning = nullptr;
		}

		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Radius(1.4f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
		if (CNavigationMgr::GetInstance()->Is_Outside(m_pGameObject->Get_CurrLevel(), m_pGameObject, 1.5f))
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
			m_iCurrAnimation++;
			m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
				m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[8].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) >= 44 && m_bShoot[1] && !m_pGameObject->Get_ModelCom()->IsNext())
	{

		if (m_bSoundOn[8] == false)
		{
			m_bSoundOn[8] = true;
			CSound_Manager::GetInstance()->PlaySoundFile(L"Effect",L"Valtan#139 (195687587).wav", 1.f);
		}
		if (m_pGameObject->Get_NearTarget() == nullptr)
		{
			
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(2.5f, 110.0f, 1.f, 11.0f);
			CSkill::ModelDesc ModelDesc = {};
			ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
			ModelDesc.iObjectID = -1;
			ModelDesc.pOwner = m_pGameObject;
			CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Valtan#173 (340544007).wav", 1.f);
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(3.f);
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				static_cast<CSkill*>(pSkill)->Set_Atk(99999);
				static_cast<CSkill*>(pSkill)->Set_Force(50.f);
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_BugGrabExplosion2", &tDesc);
		}
		else
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_BugGrabExplosion1", &tDesc);
		}
		
		m_bShoot[1] = false;
	}
	Add_Sound(0, 0, L"Effect", L"Valtan#113 (872104708)");

	Add_Sound(0, 1, L"Effect", L"Valtan#153 (153317365)", 24);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && !m_bSoundOn[2])
	{
		m_bSoundOn[2] = true;
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Valtan#154 (178464551).wav", 0.5f);
		CSound_Manager::GetInstance()->PlaySoundFile(L"Effect", L"Valtan#162 (147083337).wav", 0.5f);
	}

	Add_Sound(3, 3, L"Effect", L"Valtan#16 (800951587)");

	Add_Sound(5, 4, L"Effect", L"IwillSmashYouBugs",0 ,1.f);

	if (m_pGameObject->Get_NearTarget() == nullptr && !m_pGameObject->Get_ModelCom()->IsNext())
		Add_Sound(14, 5, L"Effect", L"Valtan#468 (401443118)");

	Add_Sound(8, 6, L"Effect", L"Valtan#43 (1015057785)");

	Add_Sound(8, 7, L"Effect", L"Valtan#135 (936435461)");

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_BugSmash::OnEnd()
{
	__super::OnEnd();
	if (m_pRushWarning != nullptr)
	{
		m_pRushWarning->EffectEnd();
		m_pRushWarning = nullptr;
	}
	static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	CSkill::ModelDesc ModelDesc = {};
	ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
	ModelDesc.iObjectID = -1;
	ModelDesc.pOwner = m_pGameObject;
	CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
	if (pSkill != nullptr)
	{
		Vec3 vPos = Get_GrabMatrix().Translation();
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(0.5f);
		pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		static_cast<CSkill*>(pSkill)->Set_Atk(0);
		static_cast<CSkill*>(pSkill)->Set_Force(20.f);
	}
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);

	Smoke_End();
}

Matrix CValtan_BT_Attack_BugSmash::Get_GrabMatrix()
{
	_uint iBoneIndex = m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand"));
	Matrix Pivot = m_pGameObject->Get_ModelCom()->Get_PivotMatrix();
	XMMATRIX BoneMatrix = m_pGameObject->Get_ModelCom()->Get_CombinedMatrix(iBoneIndex) * Pivot;

	BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
	BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
	BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

	BoneMatrix *= m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
	Matrix matWorld = BoneMatrix;

	return matWorld;
}

void CValtan_BT_Attack_BugSmash::Update_ChargeTrail(_float fTimeDelta)
{
	for (auto& iter = m_ChargeEffects.begin(); iter != m_ChargeEffects.end();)
	{
		if ((*iter)->Is_Active())
		{
			++iter;
		}
		else
		{
			iter = m_ChargeEffects.erase(iter);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex ||
		m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex ||
		m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[7].iAnimIndex)
	{
		m_fChargeTrailAcc += fTimeDelta;
		if (m_fChargeTrailAcc >= m_fChargeTrailTime)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			Matrix matWorld = Matrix::CreateFromYawPitchRoll(pGameInstance->Random_Float(0.0f, 6.28f), pGameInstance->Random_Float(0.0f, 6.28f), pGameInstance->Random_Float(0.0f, 6.28f));
			Matrix matBone = Get_GrabMatrix();
			matWorld.Translation(matBone.Translation() + matBone.Up() * 0.5f);

			auto& func = bind(&CBoss_Valtan::Load_GrabMatrix, static_cast<CBoss_Valtan*>(m_pGameObject), placeholders::_1);

			wstring szEffectName = L"VT_BugCharge";
			szEffectName += to_wstring(rand() % 2 + 1);

			TRAIL_START_OUTLIST(szEffectName, func, m_ChargeEffects);
			m_ChargeEffects.back()->Update_Pivot(matWorld);

			Safe_Release(pGameInstance);

			m_fChargeTrailAcc = 0.0f;
		}
	}
}

void CValtan_BT_Attack_BugSmash::Smoke_End()
{
	for (auto& Effect : m_SmokeEffects)
	{
		dynamic_cast<CEffect_Particle*>(Effect)->ParticleEnd();
	}
	m_SmokeEffects.clear();
}



CValtan_BT_Attack_BugSmash* CValtan_BT_Attack_BugSmash::Create(void* pArg)
{
	CValtan_BT_Attack_BugSmash* pInstance = new CValtan_BT_Attack_BugSmash;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_BugSmash");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_BugSmash::Free()
{
	__super::Free();
}
