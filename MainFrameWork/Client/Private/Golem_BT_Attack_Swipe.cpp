#include "stdafx.h"
#include "Golem_BT_Attack_Swipe.h"
#include "Monster.h"
#include "Model.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"
#include "Boss_Golem.h"
#include <Skill.h>
#include "GameInstance.h"

CGolem_BT_Attack_Swipe::CGolem_BT_Attack_Swipe()
{
}

void CGolem_BT_Attack_Swipe::OnStart()
{
	__super::OnStart(0);
	m_Shoot = true;

	m_iMaxSound = 4;
	for (size_t i = 0; i < m_iMaxSound; i++)
	{
		m_bSoundOn[i] = false;
	}
}

CBT_Node::BT_RETURN CGolem_BT_Attack_Swipe::OnUpdate(_float fTimeDelta)
{
	Add_Sound(0, 0, TEXT("Effect"), TEXT("Rook_9"), 15, 0.5f);
	Add_Sound(0, 1, TEXT("Effect"), TEXT("Rook_42"), 18, 0.5f);
	Add_Sound(0, 2, TEXT("Effect"), TEXT("Rook_10"), 42, 0.5f);
	Add_Sound(0, 3, TEXT("Effect"), TEXT("Rook_43"), 48, 0.5f);

	if (18 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
	{
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, true);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Atk(14);
		dynamic_cast<CBoss*>(m_pGameObject)->Set_Force(10.5f);
	}
	if (26 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex))
		dynamic_cast<CMonster*>(m_pGameObject)->Set_Collider_Active((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS, false);
	if (43 <= m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) && m_Shoot)
	{
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;


		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Golem_Swipe", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 1.25f;
			vLook = Vec3::TransformNormal(vLook,Matrix::CreateFromAxisAngle(Vec3(0.f, 1.f, 0.f), XMConvertToRadians(-35.f)));
			vLook.Normalize();
			vPos.y = 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			m_Shoot = false;
		}

	}
	return BT_RUNNING;
}

void CGolem_BT_Attack_Swipe::OnEnd()
{
	dynamic_cast<CMonster*>(m_pGameObject)->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	__super::OnEnd();

}



CGolem_BT_Attack_Swipe* CGolem_BT_Attack_Swipe::Create(void* pArg)
{
	CGolem_BT_Attack_Swipe* pInstance = new CGolem_BT_Attack_Swipe;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CGolem_BT_Attack_Swipe");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CGolem_BT_Attack_Swipe::Free()
{
	__super::Free();
}
