#include "stdafx.h"
#include "King_BT_Attack_Erruption.h"
#include "Monster.h"
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
	m_Shoot = true;

	m_bLookatRight = CGameInstance::GetInstance()->Random_Coin(0.5f);
}

CBT_Node::BT_RETURN CKing_BT_Attack_Erruption::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) == 50 && m_Shoot)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matPivot = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vOriginalPos = matPivot.Translation();
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot *=	XMMatrixRotationY(XMConvertToRadians(60.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		matPivot *= XMMatrixRotationY(XMConvertToRadians(120.f));
		matPivot.Translation(vOriginalPos);
		desc.pPivotMatrix = &matPivot;
		EFFECT_START(TEXT("Chaos_King_Pizza"), &desc)

		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		//ModelDesc.strFileName
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		if (m_bLookatRight)
		{
			Vec3 vRight = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_RIGHT);
			vRight.Normalize();
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{

				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vRight);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{

				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(-vRight);
			}
		}
		else
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			Vec3 Look = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			Look.Normalize();
			if (pSkill != nullptr)
			{

				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(Look);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_King_Eruption", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(-Look);
			}
		}
		m_Shoot = false;
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
