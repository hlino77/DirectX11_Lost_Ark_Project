#include "stdafx.h"
#include "Valtan_BT_Attack_FrontBackWave.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include "Effect.h"

CValtan_BT_Attack_FrontBackWave::CValtan_BT_Attack_FrontBackWave()
{
}

void CValtan_BT_Attack_FrontBackWave::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_FrontBackWave::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 36 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.1f, 90.0f, 0.1f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Add_Sound( L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#64 (963708198)");
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_FBChop1", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 66 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#65 (7989749)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.1f, 90.0f, 0.1f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos -= vLook * 0.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vLook);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_FBChop2", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 36 && m_bShoot[2])
	{
		m_bShoot[2] = false;
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#104 (160150074)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.1f, 90.0f, 0.1f, 11.0f);
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
			vPos += vLook * 2.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider((_uint)LAYER_COLLIDER::LAYER_SKILL_BOSS)->Set_Radius(2.5f);
		}

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_FBChop3", &tDesc);
	}
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 80 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_iCurrAnimation++;
		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex,
			m_vecAnimDesc[m_iCurrAnimation].fChangeTime, m_vecAnimDesc[m_iCurrAnimation].iStartFrame,
			m_vecAnimDesc[m_iCurrAnimation].iChangeFrame);
	}
	{
		Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#17 (437813884)",23);
		Add_Sound(0, 1, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#44 (813796387)",60);
		Add_Sound(2, 2, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#91 (483284436)",23);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_FrontBackWave::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_FrontBackWave* CValtan_BT_Attack_FrontBackWave::Create(void* pArg)
{
	CValtan_BT_Attack_FrontBackWave* pInstance = new CValtan_BT_Attack_FrontBackWave;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FrontBackWave");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_FrontBackWave::Free()
{
	__super::Free();
}
