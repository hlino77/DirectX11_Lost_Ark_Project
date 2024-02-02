#include "stdafx.h"
#include "Valtan_BT_Attack_Attack11.h"
#include "Boss_Valtan.h"
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"

CValtan_BT_Attack_Attack11::CValtan_BT_Attack_Attack11()
{
}

void CValtan_BT_Attack_Attack11::OnStart()
{
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		__super::OnStart(1);
	else
		__super::OnStart();
	m_bShoot = true;		 
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Attack11::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 45)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(2.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(15.f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 50)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 56 && m_bShoot)
	{
		m_bShoot = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.05f, 50.0f, 0.1f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SeismicWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 2.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}
		if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			static_cast<CMonster*>(m_pGameObject)->Set_Die();
			static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.5f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 55 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex,
			m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame,
			m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}

	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
	{
		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 35 && !static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
			static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(true);

		if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 55 && static_cast<CBoss*>(m_pGameObject)->Is_CounterSkill())
			static_cast<CBoss*>(m_pGameObject)->Set_CounterSkill(false);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Attack11::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_Attack11* CValtan_BT_Attack_Attack11::Create(void* pArg)
{
	CValtan_BT_Attack_Attack11* pInstance = new CValtan_BT_Attack_Attack11;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Attack0");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Attack11::Free()
{
	__super::Free();
}
