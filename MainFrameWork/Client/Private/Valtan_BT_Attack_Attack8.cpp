#include "stdafx.h"
#include "Valtan_BT_Attack_Attack8.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include <Skill_Valtan_DoughnutFirstHit.h>
#include <Skill_Valtan_DoughnutSecondHit.h>
#include "ColliderSphere.h"
#include "GameInstance.h"

CValtan_BT_Attack_Attack8::CValtan_BT_Attack_Attack8()
{
}

void CValtan_BT_Attack_Attack8::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack8::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 49)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(2.f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(15.f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 59)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 67)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 73)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 92)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 94)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 94 && m_bShoot[0])
	{
		m_bShoot[0] = false;
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
			vPos += vLook * 2.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill_Valtan_DoughnutFirstHit*>(pSkill)->Set_DoughnutRadii(7.5f,2.3f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 68 && m_bShoot[1])
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
			static_cast<CSkill_Valtan_DoughnutFirstHit*>(pSkill)->Set_DoughnutRadii(6.f, 2.3f);
		}
		pSkill == nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_DoughnutSecondHit", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill_Valtan_DoughnutSecondHit*>(pSkill)->Set_DoughnutRadii(9.f, 5.3f);
		}
	}


	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack8::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack8* CValtan_BT_Attack_Attack8::Create(void* pArg)
{
	CValtan_BT_Attack_Attack8* pInstance = new CValtan_BT_Attack_Attack8;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack8::Free()
{
	__super::Free();
}
