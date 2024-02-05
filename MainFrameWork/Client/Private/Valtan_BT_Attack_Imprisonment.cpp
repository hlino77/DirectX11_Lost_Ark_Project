#include "stdafx.h"
#include "Valtan_BT_Attack_Imprisonment.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include <Player.h>

CValtan_BT_Attack_Imprisonment::CValtan_BT_Attack_Imprisonment()
{
}

void CValtan_BT_Attack_Imprisonment::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Imprisonment::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[0].iAnimIndex) - 3 && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		CPlayer* pTarget = static_cast<CPlayer*>(CGameInstance::GetInstance()->Find_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER, (_int)m_pGameObject->Get_TargetPos().x));
		if(pTarget != nullptr)
			pTarget->Set_State(L"Stop");
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 34 && m_bShoot)
	{
		m_bShoot = false;
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
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(7.5f);
			static_cast<CSkill*>(pSkill)->Set_Force(-16.f);
		}

	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) < 30)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.7f);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > 27)
	{
		for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_MONSTER))
		{
			if (pGameObject->Get_ObjectTag() == L"Monster_Prison"&& pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)->IsActive())
			{
				CSkill::ModelDesc ModelDesc = {};
				ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
				ModelDesc.iObjectID = -1;
				ModelDesc.pOwner = m_pGameObject;

				CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereTerm", &ModelDesc);
				if (pSkill != nullptr)
				{
					Vec3 vPos = pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					Vec3 vLook = pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
					vLook.Normalize();
					pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(5.f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.f);
					static_cast<CSkill*>(pSkill)->Set_Force(10.f);
				}
				pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_BODY_MONSTER)->SetActive(false);

			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Imprisonment::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Imprisonment* CValtan_BT_Attack_Imprisonment::Create(void* pArg)
{
	CValtan_BT_Attack_Imprisonment* pInstance = new CValtan_BT_Attack_Imprisonment;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Imprisonment");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Imprisonment::Free()
{
	__super::Free();
}
