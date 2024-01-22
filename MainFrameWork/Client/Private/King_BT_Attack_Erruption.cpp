#include "stdafx.h"
#include "King_BT_Attack_Erruption.h"
#include "Boss.h"
#include "Transform.h"
#include "Model.h"
#include "Skill.h"
#include "GameInstance.h"
#include "Effect_Manager.h"

CKing_BT_Attack_Erruption::CKing_BT_Attack_Erruption()
{
}

void CKing_BT_Attack_Erruption::OnStart()
{
	__super::OnStart(0);
	static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(20);
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
	m_Shoot[0] = true;
	m_Shoot[1] = true;
	m_Shoot[2] = true;
}

CBT_Node::BT_RETURN CKing_BT_Attack_Erruption::OnUpdate(const _float& fTimeDelta)
{
	if (m_Shoot[2] && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		m_Shoot[2] = false;
		static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(20);
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
	}
	if (m_fLoopTime > m_vecAnimDesc[3].fMaxLoopTime)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(0);
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_Shoot[0])
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vOriginalPos = matWorld.Translation();
		Matrix matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(-30.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(30.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(180.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vSkillLook = Vec3::TransformNormal(vLook, XMMatrixRotationY(XMConvertToRadians(210.f)));
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vSkillLook);

			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vSkillLook = Vec3::TransformNormal(vLook, XMMatrixRotationY(XMConvertToRadians(150.f)));
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vSkillLook);
			}
		
		m_Shoot[0] = false;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_fLoopTime>3.f && m_Shoot[1])
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vOriginalPos = matWorld.Translation();
		Matrix matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(60.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(120.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot = matWorld * XMMatrixRotationY(XMConvertToRadians(270.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
		Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vRight);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vSkillLook = Vec3::TransformNormal(vRight, XMMatrixRotationY(XMConvertToRadians(150.f)));
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vSkillLook);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_King_Eruption", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vSkillLook = Vec3::TransformNormal(vRight, XMMatrixRotationY(XMConvertToRadians(210.f)));
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vSkillLook);
		}


		m_Shoot[1] = false;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Erruption::OnEnd()	
{
	__super::OnEnd();
	static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(0);
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
}



CKing_BT_Attack_Erruption* CKing_BT_Attack_Erruption::Create(void* pArg)
{
	CKing_BT_Attack_Erruption* pInstance = new CKing_BT_Attack_Erruption;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CKing_BT_Attack_Erruption");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CKing_BT_Attack_Erruption::Free()
{
	__super::Free();
}
