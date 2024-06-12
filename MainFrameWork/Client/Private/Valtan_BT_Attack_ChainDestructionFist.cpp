#include "stdafx.h"
#include "Valtan_BT_Attack_ChainDestructionFist.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"
#include <Skill.h>	
#include <Skill_Crystal.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect_Particle.h"
#include "Effect.h"
#include "Effect_Trail.h"

void CValtan_BT_Attack_ChainDestructionFist::Load_TrailEffectMatrix(Matrix& matWorld)
{
	matWorld.Translation(m_pGameObject->Get_TransformCom()->Get_WorldMatrix().Translation());
	matWorld.m[3][1] += 1.5f;
}

CValtan_BT_Attack_ChainDestructionFist::CValtan_BT_Attack_ChainDestructionFist()
{
}

void CValtan_BT_Attack_ChainDestructionFist::OnStart()
{
	__super::OnStart();
	static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(false);
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;
	m_bShoot[4] = true;
	m_bShoot[5] = true;
	m_bShoot[6] = true;
	m_bShoot[7] = true;
	m_bShoot[8] = true;
	m_bShoot[9] = true;
	m_bShoot[10] = true;

	m_Door.clear();
	m_DoorParticle.clear();

	m_bLandWarning[0] = false;
	m_bLandWarning[1] = false;

	m_vTargetPos = Vec3(100.0f, 0.19f, 100.0f);

	m_bPizzaWarning[0] = false;
	m_bPizzaWarning[1] = false;
	m_bPizzaWarning[2] = false;
	m_bPizzaWarning[3] = false;

	m_bLastChopWarning = false;
	m_bLastRing = false;

	m_bLastShoutWarning = false;

	m_fChargeTrailAcc = 0.0f;
	m_fChargeTrailTime = 0.05f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_ChainDestructionFist::OnUpdate(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 30)
	{
		if (m_Door.empty() == true)
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			matWorld.Translation(matWorld.Translation() + matWorld.Backward() * 9.0f);

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;

			EFFECT_START_OUTLIST(L"VT_ChainDoor", &tDesc, m_Door);
			EFFECT_START_OUTLIST(L"VT_ChainDoorParticle", &tDesc, m_DoorParticle);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(true);
		static_cast<CBoss*>(m_pGameObject)->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vTargetPos);

		if (m_Door.empty() == false)
		{
			for (auto& Effect : m_Door)
			{
				Effect->EffectEnd();
			}
			m_Door.clear();

			for (auto& Particle : m_DoorParticle)
			{
				dynamic_cast<CEffect_Particle*>(Particle)->ParticleEnd();
			}
			m_DoorParticle.clear();
		}


		if (m_bLandWarning[0] == false && m_fLoopTime >= 0.5f)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainWarning1", &tDesc);
			m_bLandWarning[0] = true;
		}

		if (m_bLandWarning[1] == false && m_fLoopTime >= 1.0f)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainWarning2", &tDesc);
			m_bLandWarning[1] = true;
		}
	}

	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) >= 95 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex, m_vecAnimDesc[m_iCurrAnimation].fChangeTime,
			m_vecAnimDesc[m_iCurrAnimation].iStartFrame, m_vecAnimDesc[m_iCurrAnimation].iChangeFrame, m_vecAnimDesc[m_iCurrAnimation].fRootDist, m_vecAnimDesc[m_iCurrAnimation].IsRootRot);
		for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL))
		{
			if (pGameObject->Get_ObjectTag() == L"Skill_Crystal")
			{
				dynamic_cast<CSkill_Crystal*>(pGameObject)->Set_Explosion(true);
			}
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 25 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(1.f, 110.0f, 1.8f, 11.0f);
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
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(4.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainChop", &tDesc);

	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 36 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		Add_Sound(L"Effect", L"Valtan#263 (334042417)");
		Add_Sound(L"Effect", L"Valtan#274 (200670410)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainPChop1", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 66 && m_bShoot[2])
	{
		m_bShoot[2] = false;
		Add_Sound(L"Effect",L"Valtan#264 (944100163)");
		Add_Sound(L"Effect",L"Valtan#275 (247868684)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(20.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainPChop2", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 96 && m_bShoot[3])
	{
		Add_Sound(L"Effect",L"Valtan#262 (50890152)");
		Add_Sound(L"Effect",L"Valtan#276 (895471458)");
		m_bShoot[3] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vRight);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainPChop3", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 126 && m_bShoot[4])
	{
		m_bShoot[4] = false;	
		Add_Sound(L"Effect",L"Valtan#263 (334042417)");
		Add_Sound(L"Effect",L"Valtan#255 (401940576)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vRight);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(20.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainPChop4", &tDesc);
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex);

		if (m_bPizzaWarning[0] == false && iAnimFrame >= 6)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainPWarning1", &tDesc);

			m_bPizzaWarning[0] = true;
		}

		if (m_bPizzaWarning[1] == false && iAnimFrame >= 36)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainPWarning2", &tDesc);

			m_bPizzaWarning[1] = true;
		}

		if (m_bPizzaWarning[2] == false && iAnimFrame >= 66)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainPWarning3", &tDesc);

			m_bPizzaWarning[2] = true;
		}

		if (m_bPizzaWarning[3] == false && iAnimFrame >= 96)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainPWarning4", &tDesc);

			m_bPizzaWarning[3] = true;
		}

	}


	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 6 && !m_pGameObject->Get_ModelCom()->IsNext() && m_bShoot[5])
	{
		m_bShoot[5] = false;
		Add_Sound(L"Effect", L"Valtan#36 (407979323)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			vLook.Normalize();
			vPos += vLook * 2.f;
			vPos += vRight * 0.7f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(1.5f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainFistRight", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 12 && !m_pGameObject->Get_ModelCom()->IsNext() && m_bShoot[6])
	{
		Add_Sound(L"Effect", L"Valtan#37 (464077464)");
		m_bShoot[6] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			vLook.Normalize();
			vPos += vLook * 2.f;
			vPos -= vRight * 0.7f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(1.5f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainFistLeft", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex &&
		m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_bShoot[5] = true;
		m_bShoot[6] = true;
		if ((m_iCurrAnimation == 7&& m_bShoot[9])|| (m_iCurrAnimation == 9 && m_bShoot[10]))
		{
			if(m_iCurrAnimation == 7 && m_bShoot[9])
				m_bShoot[9] = false;
			if (m_iCurrAnimation == 9 && m_bShoot[10])
				m_bShoot[10] = false;
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
						pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
						pSkill->Get_TransformCom()->LookAt_Dir(vLook);
						pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
						static_cast<CSkill*>(pSkill)->Set_Atk(30);
						static_cast<CSkill*>(pSkill)->Set_Force(42.f);
						static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#262 (50890152).wav");
						static_cast<CSkill*>(pSkill)->Set_BlinkTime(3.f);
						static_cast<CSkill*>(pSkill)->Set_LastTime(3.2f);
						Matrix matWorld;
						matWorld.Translation(vPos);
						CEffect_Manager::EFFECTPIVOTDESC tDesc;
						tDesc.pPivotMatrix = &matWorld;
						EFFECT_START(L"VT_ChainPlayerChop", &tDesc);
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
						static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#262 (50890152).wav");
						static_cast<CSkill*>(pSkill)->Set_BlinkTime(3.f);
						static_cast<CSkill*>(pSkill)->Set_LastTime(3.2f);
						Matrix matWorld;
						matWorld.Translation(vPos);
						CEffect_Manager::EFFECTPIVOTDESC tDesc;
						tDesc.pPivotMatrix = &matWorld;
						EFFECT_START(L"VT_ChainPlayerChop", &tDesc);
					}
				}
		}
	}

	if (m_bLastChopWarning == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[11].iAnimIndex)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainLastWarning", &tDesc);

		m_bLastChopWarning = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[11].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[11].iAnimIndex) >= 31 && m_bShoot[7])
	{
		Add_Sound(L"Effect", L"Valtan#106 (917948767)");
		Add_Sound(L"Effect", L"Valtan#284 (865380591)");
		m_bShoot[7] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(1.f, 110.0f, 1.8f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;


		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainLastChop", &tDesc);

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 1.8f;
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
			vPos += vLook * 1.8f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_DoughnutRadii(9.f, 4.5f);
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#107 (1071718533).wav");
			static_cast<CSkill*>(pSkill)->Set_BlinkTime(2.f);
			static_cast<CSkill*>(pSkill)->Set_LastTime(2.2f);
			static_cast<CSkill*>(pSkill)->Set_SafeZonePierce(true);		
		}

		Vec3 vDir = -m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);;
		vDir.Normalize();
		Matrix matRotation90 = Matrix::CreateRotationY(XMConvertToRadians(90.f));

		for (size_t i = 0; i < 3; i++)
		{
			Vec3 vPosition = static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition();
			vDir = Vec3::TransformNormal(vDir, matRotation90);
			vDir.y = 0.f;
			vDir.Normalize();
			vPosition += vDir * 5.f;

			{
				Matrix matWorld;
				matWorld.Translation(vPosition);
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &matWorld;
				EFFECT_START(L"VT_FTStoneSpawn", &tDesc);
			}

			vPosition.y = - 3.f;
			Add_Stone(vPosition);
		}

	}

	if (m_bLastRing == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[11].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[11].iAnimIndex) >= 50)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainLastWarning2", &tDesc);
		EFFECT_START(L"VT_ChainLastRing", &tDesc);

		m_bLastRing = true;
	}

	Update_Trail(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[12].iAnimIndex)
	{
		if (m_bLastShoutWarning == false)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ChainLastWarning3", &tDesc);
			m_bLastShoutWarning = true;
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[14].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[14].iAnimIndex) >= 20 && !m_pGameObject->Get_ModelCom()->IsNext() && m_bShoot[8])
	{
		m_bShoot[8] = false;
		Add_Sound(L"Effect", L"Valtan#283 (267591526)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(1.f, 0.7f);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(1.f, 0.7f);
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(30.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(45.f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ChainLastShout", &tDesc);
	}		
	Add_Sound(0, 0,L"Effect", L"Valtan#465 (1004371698)");
	Add_Sound(0, 1,L"Effect", L"Valtan#252 (770162512)", 27);
	Add_Sound(1, 2,L"Effect", L"Valtan#254 (893264685)", 11);
	Add_Sound(3, 3,L"Effect", L"Valtan#275 (247868684)", 23);
	Add_Sound(3, 4,L"Effect", L"Valtan#260 (516063166)", 23);
	Add_Sound(5, 5,L"Effect", L"Valtan#279 (278541855)");
	Add_Sound(15, 6,L"Effect", L"Valtan#305 (20427736)");
	Add_Sound(15, 7,L"Effect", L"Valtan#268 (165809185)",40);

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_ChainDestructionFist::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}

void CValtan_BT_Attack_ChainDestructionFist::Update_Trail(_float fTimeDelta)
{
	for (auto& iter = m_Trails.begin(); iter != m_Trails.end();)
	{
		if ((*iter)->Is_Active())
		{
			++iter;
		}
		else
		{
			iter = m_Trails.erase(iter);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[12].iAnimIndex ||
		m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[13].iAnimIndex)
	{
		m_fChargeTrailAcc += fTimeDelta;
		if (m_fChargeTrailAcc >= m_fChargeTrailTime)
		{
			CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);
			Matrix matWorld = Matrix::CreateFromYawPitchRoll(pGameInstance->Random_Float(0.0f, 6.28f), pGameInstance->Random_Float(0.0f, 6.28f), pGameInstance->Random_Float(0.0f, 6.28f));
			Load_TrailEffectMatrix(matWorld);

			auto& func = bind(&CValtan_BT_Attack_ChainDestructionFist::Load_TrailEffectMatrix, this, placeholders::_1);

			wstring szEffectName = L"VT_ChainTrail";
			szEffectName += to_wstring(rand() % 2 + 1);

			TRAIL_START_OUTLIST(szEffectName, func, m_Trails);
			m_Trails.back()->Update_Pivot(matWorld);

			Safe_Release(pGameInstance);

			m_fChargeTrailAcc = 0.0f;
		}
	}
}



CValtan_BT_Attack_ChainDestructionFist* CValtan_BT_Attack_ChainDestructionFist::Create(void* pArg)
{
	CValtan_BT_Attack_ChainDestructionFist* pInstance = new CValtan_BT_Attack_ChainDestructionFist;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_ChainDestructionFist");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_ChainDestructionFist::Free()
{
	__super::Free();
}
