#include "stdafx.h"
#include "Valtan_BT_Attack_Attack15.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"

CValtan_BT_Attack_Attack15::CValtan_BT_Attack_Attack15()
{
}

void CValtan_BT_Attack_Attack15::OnStart()
{
	__super::OnStart();

}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack15::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > 27)
	{
		for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_MONSTER))
		{
			if (pGameObject->Get_ObjectTag() == L"Monster_Prison")
			{
				CSkill::ModelDesc ModelDesc = {};
				ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
				ModelDesc.iObjectID = -1;
				ModelDesc.pOwner = m_pGameObject;


				CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_SKill_Valtan_SphereTerm", &ModelDesc);
				if (pSkill != nullptr)
				{
					Vec3 vPos = pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					Vec3 vLook = pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
					vLook.Normalize();
					pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(3.f);
					static_cast<CSkill*>(pSkill)->Set_LastTime(1.2f);
					static_cast<CSkill*>(pSkill)->Set_BlinkTime(1.f);
					static_cast<CSkill*>(pSkill)->Set_Force(10.f);
				}
				static_cast<CMonster*>(pGameObject)->Set_Die();

			}
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack15::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_Attack15* CValtan_BT_Attack_Attack15::Create(void* pArg)
{
	CValtan_BT_Attack_Attack15* pInstance = new CValtan_BT_Attack_Attack15;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack15::Free()
{
	__super::Free();
}
