#include "stdafx.h"
#include "Valtan_BT_Attack_Attack23.h"
#include "Boss.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"
#include <Skill.h>	

CValtan_BT_Attack_Attack23::CValtan_BT_Attack_Attack23()
{
}

void CValtan_BT_Attack_Attack23::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;
	m_bShoot[4] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack23::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->IsNext())
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 25 && m_bShoot[0])
	{
		m_bShoot[0] = false;
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

	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 36 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(1.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(45.f, -45.f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 66 && m_bShoot[2])
	{
		m_bShoot[2] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
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
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(1.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(45.f, -45.f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 96 && m_bShoot[3])
	{
		m_bShoot[3] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vRight);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(1.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(45.f, -45.f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 126 && m_bShoot[4])
	{
		m_bShoot[4] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
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
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(1.f, 0.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(45.f, -45.f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 6 && m_bShoot[5])
	{
		m_bShoot[5] = false;
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
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
		}

	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 25 && m_bShoot[0])
	{
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
			vLook.Normalize();
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(20);
			static_cast<CSkill*>(pSkill)->Set_Force(41.f);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack23::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack23* CValtan_BT_Attack_Attack23::Create(void* pArg)
{
	CValtan_BT_Attack_Attack23* pInstance = new CValtan_BT_Attack_Attack23;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack23::Free()
{
	__super::Free();
}
