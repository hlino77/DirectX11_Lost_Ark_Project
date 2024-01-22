#include "stdafx.h"
#include "Valtan_BT_Attack_Attack6.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill_Valtan_PizzaTerm.h>

CValtan_BT_Attack_Attack6::CValtan_BT_Attack_Attack6()
{
}

void CValtan_BT_Attack_Attack6::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack6::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[2].iAnimIndex) - 3 && m_bShoot)
	{
		m_bShoot = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		for (size_t i = 0; i < 3; i++)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.f;
			vLook = Vec3::TransformNormal(vLook, XMMatrixRotationY(XMConvertToRadians(60.f* (_float)i)));
			vLook.Normalize();
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaTerm", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);

				static_cast<CSkill*>(pSkill)->Set_Atk(20.f);
				static_cast<CSkill*>(pSkill)->Set_Force(40.f);
				static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f+ (_float)i);
				static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.f + (_float)i);
			}

			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaTerm", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(-vLook);
				static_cast<CSkill*>(pSkill)->Set_Atk(20.f);
				static_cast<CSkill*>(pSkill)->Set_Force(40.f);
				static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f + (_float)i);
				static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.f + (_float)i);
			}
		}
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack6::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack6* CValtan_BT_Attack_Attack6::Create(void* pArg)
{
	CValtan_BT_Attack_Attack6* pInstance = new CValtan_BT_Attack_Attack6;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack6::Free()
{
	__super::Free();
}
