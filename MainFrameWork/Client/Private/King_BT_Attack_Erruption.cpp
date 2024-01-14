#include "stdafx.h"
#include "King_BT_Attack_Erruption.h"
#include "Monster.h"
#include "Transform.h"
#include "Model.h"
#include "Skill.h"
#include "GameInstance.h"

CKing_BT_Attack_Erruption::CKing_BT_Attack_Erruption()
{
}

void CKing_BT_Attack_Erruption::OnStart()
{
	__super::OnStart(0);
	m_Shoot[0] = true;
	m_Shoot[1] = true;
}

CBT_Node::BT_RETURN CKing_BT_Attack_Erruption::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_Shoot[0])
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		//ModelDesc.strFileName
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			if (pSkill != nullptr)
			{
				Vec3 vLeftFront = Vec3::TransformNormal(vLook, Matrix::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), -22.5f));
				vLeftFront.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLeftFront);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vRightFront = Vec3::TransformNormal(vLook, Matrix::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), 22.5f));
				vRightFront.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vRightFront);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(-vLook);
			}
		
		m_Shoot[0] = false;
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_fLoopTime>3.f && m_Shoot[1])
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		//ModelDesc.strFileName
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
		Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
		vRight.Normalize();
		if (pSkill != nullptr)
		{
			Vec3 vLeftFront = Vec3::TransformNormal(vRight, Matrix::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), -22.5f));
			vLeftFront.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLeftFront);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vRightFront = Vec3::TransformNormal(vRight, Matrix::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), 22.5f));
			vRightFront.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vRightFront);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vRight);
		}

		m_Shoot[1] = false;
	}
	return __super::OnUpdate(fTimeDelta);
}

void CKing_BT_Attack_Erruption::OnEnd()
{
	__super::OnEnd();
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
