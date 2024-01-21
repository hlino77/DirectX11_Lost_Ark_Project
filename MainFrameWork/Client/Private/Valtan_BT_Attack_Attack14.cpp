#include "stdafx.h"
#include "Valtan_BT_Attack_Attack14.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include "ColliderSphere.h"
#include <SKill_Valtan_DoughnutTerm2sec.h>

CValtan_BT_Attack_Attack14::CValtan_BT_Attack_Attack14()
{
}

void CValtan_BT_Attack_Attack14::OnStart()
{
	__super::OnStart();
	if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
		m_iAttackCount = 9;
	if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2)
		m_iAttackCount = 3;
	m_fAttackTerm = 0.f;
	m_bShoot =true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack14::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
	}
	else if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2&& m_fLoopTime < m_vecAnimDesc[1].fMaxLoopTime-1.f)
	{
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, static_cast<CBoss*>(m_pGameObject)->Get_NearTarget_Position());
	}
	if (m_fLoopTime > m_vecAnimDesc[1].fMaxLoopTime)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(m_vecAnimDesc[2].strAnimName, m_vecAnimDesc[2].fChangeTime, m_vecAnimDesc[2].iStartFrame, m_vecAnimDesc[2].iChangeFrame, m_vecAnimDesc[2].fAnimSpeed);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_fAttackTerm += fTimeDelta;
		_float fMaxTerm = 0.f;
		if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2)
			fMaxTerm = 1.f;
		else
			fMaxTerm = 0.75f;
		if (m_fAttackTerm > fMaxTerm && m_iAttackCount >0)
		{
			m_iAttackCount--;
			m_fAttackTerm = 0.f;
			if (m_iAttackCount == 0)
			{
				m_fLoopTime = m_vecAnimDesc[1].fMaxLoopTime - 2.f;
			}

			vector<CGameObject*> vecTargets = CGameInstance::GetInstance()->Find_GameObjects(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_PLAYER);
			if (!vecTargets.empty())
				for (auto& Object : vecTargets)
				{
					CSkill::ModelDesc ModelDesc = {};
					ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
					ModelDesc.iObjectID = -1;
					ModelDesc.pOwner = m_pGameObject;


					CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_SphereTerm", &ModelDesc);
					if (pSkill != nullptr)
					{
						Vec3 vPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
						Vec3 vLook = Object->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
						vLook.Normalize();	
						pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
						pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					}
				}
			vecTargets = CGameInstance::GetInstance()->Find_GameObjects(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_PLAYER);
			if (!vecTargets.empty())
				for (auto& Object : vecTargets)
				{
					CSkill::ModelDesc ModelDesc = {};
					ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
					ModelDesc.iObjectID = -1;
					ModelDesc.pOwner = m_pGameObject;


					CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_SphereTerm", &ModelDesc);
					if (pSkill != nullptr)
					{
						Vec3 vPos = Object->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
						Vec3 vLook = Object->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
						vLook.Normalize();
						pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
						pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					}
				}
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 8 && m_bShoot)
	{
		m_bShoot = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 1)
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(8.f);
			}
		}
		else
		{
			CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_SphereInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(5.f);
			}
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_DoughnutTerm2sec", &ModelDesc);
			if (pSkill != nullptr)
			{
				Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
				Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
				vLook.Normalize();
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				static_cast<CSKill_Valtan_DoughnutTerm2sec*>(pSkill)->Set_DoughnutRadii(12.f,4.3f);
			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack14::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack14* CValtan_BT_Attack_Attack14::Create(void* pArg)
{
	CValtan_BT_Attack_Attack14* pInstance = new CValtan_BT_Attack_Attack14;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack14::Free()
{
	__super::Free();
}
