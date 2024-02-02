#include "stdafx.h"
#include "Valtan_BT_Attack_Attack17_1.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"


CValtan_BT_Attack_Attack17_1::CValtan_BT_Attack_Attack17_1()
{
}

void CValtan_BT_Attack_Attack17_1::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack17_1::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_fLoopTime > 1.8f && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
		static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	if (m_iCurrAnimation == 2 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 31 && m_bShoot)
	{
		m_bShoot = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.5f, 50.0f, 0.1f, 10.0f);
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
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(20.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(50);
			static_cast<CSkill*>(pSkill)->Set_Force(42.f);
		}

	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack17_1::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	if (static_cast<CBoss*>(m_pGameObject)->Get_Action() == L"Action_Counter")
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SilenceSphere", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(6.f);
		}
	}
}


CValtan_BT_Attack_Attack17_1* CValtan_BT_Attack_Attack17_1::Create(void* pArg)
{
	CValtan_BT_Attack_Attack17_1* pInstance = new CValtan_BT_Attack_Attack17_1;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack17_1::Free()
{
	__super::Free();
}
