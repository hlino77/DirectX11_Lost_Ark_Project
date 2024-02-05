#include "stdafx.h"
#include "Valtan_BT_Attack_GhostGrab.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include <Skill.h>
#include "GameInstance.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_GhostGrab::CValtan_BT_Attack_GhostGrab()
{
}

void CValtan_BT_Attack_GhostGrab::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_GhostGrab::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 42)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Radius(2.f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Offset(Vec3(0.f, 1.3f, -1.1f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand")));
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 52)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 20&& m_bShoot)
	{
		m_bShoot = false;
		CSkill::ModelDesc ModelDesc = {};
			ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
			ModelDesc.iObjectID = -1;
			ModelDesc.pOwner = m_pGameObject;


			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_Breath", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			}
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_GhostGrab::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_GhostGrab* CValtan_BT_Attack_GhostGrab::Create(void* pArg)
{
	CValtan_BT_Attack_GhostGrab* pInstance = new CValtan_BT_Attack_GhostGrab;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_GhostGrab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_GhostGrab::Free()
{
	__super::Free();
}
