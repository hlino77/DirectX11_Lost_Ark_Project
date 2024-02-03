#include "stdafx.h"
#include "Valtan_BT_Attack_Attack13.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"


CValtan_BT_Attack_Attack13::CValtan_BT_Attack_Attack13()
{
}

void CValtan_BT_Attack_Attack13::OnStart()
{
	__super::OnStart();
	m_bOutSide = m_pGameObject->Get_TargetPos().x;
	m_bShoot[0] = true;
	m_bShoot[1] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack13::OnUpdate(const _float& fTimeDelta)
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
		}
	}
	if (m_bOutSide && m_iCurrAnimation == 2)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.05f, 0.7f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 7 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 110.0f, 0.5f, 10.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

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
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(5.f);
			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}


void CValtan_BT_Attack_Attack13::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack13* CValtan_BT_Attack_Attack13::Create(void* pArg)
{
	CValtan_BT_Attack_Attack13* pInstance = new CValtan_BT_Attack_Attack13;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack13::Free()
{
	__super::Free();
}
