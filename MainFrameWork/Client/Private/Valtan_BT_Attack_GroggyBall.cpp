#include "stdafx.h"
#include "Valtan_BT_Attack_GroggyBall.h"
#include "Model.h"
#include "Transform.h"
#include "NavigationMgr.h"
#include <Boss_Valtan.h>
#include <Skill.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include <Effect_Manager.h>
#include "Effect.h"
#include "UI_Boss_SpecialGroggy.h"

CValtan_BT_Attack_GroggyBall::CValtan_BT_Attack_GroggyBall()
{
}

void CValtan_BT_Attack_GroggyBall::OnStart()
{
	__super::OnStart(0);
	m_bShoot = true;
	m_bShield = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_GroggyBall::OnUpdate(const _float& fTimeDelta)
{

	if (!m_bShield && m_iCurrAnimation == 1 && !m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount() == 0)
	{
		m_bShield = true;
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Shield(20902324);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START_OUTLIST(L"VT_Shield", &tDesc, m_vecEffect);
		static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(150);
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
		static_cast<CBoss*>(m_pGameObject)->Get_GroggyUI()->Set_Active(true);
	}
	if (false == m_vecEffect.empty()&&(static_cast<CBoss_Valtan*>(m_pGameObject)->Get_Shield() < 1 || m_iCurrAnimation == 2))
	{		
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Shield(0);
		m_vecEffect.front()->EffectEnd();
		m_vecEffect.clear();
	}
	if (m_iCurrAnimation == 2 && !m_pGameObject->Get_ModelCom()->IsNext() && static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount() > 0)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Shield(0);
		static_cast<CBoss*>(m_pGameObject)->Set_Invincible(true);
		static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(0);
		static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
		static_cast<CBoss*>(m_pGameObject)->Get_GroggyUI()->Set_Active(false);
	}
	if (m_iCurrAnimation == 2 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 35 && m_bShoot)
	{
		m_bShoot = false;
		Add_Sound(L"Effect", L"Valtan#396 (63667459)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(1.f, 110.0f, 3.f, 11.0f);
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
			static_cast<CSkill*>(pSkill)->Set_SphereRadius(30.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(99999);
			static_cast<CSkill*>(pSkill)->Set_Force(55.f);
		}
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_Wipe3", &tDesc);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_GroggyBall::OnEnd()
{
	static_cast<CBoss*>(m_pGameObject)->Set_Invincible(false);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Shield(0);
	static_cast<CBoss*>(m_pGameObject)->Set_MaxGroggyCount(0);
	static_cast<CBoss*>(m_pGameObject)->Set_GroggyCount(static_cast<CBoss*>(m_pGameObject)->Get_MaxGroggyCount());
	if (false == m_vecEffect.empty())
	{
		m_vecEffect.front()->EffectEnd();
		m_vecEffect.clear();
	}
	__super::OnEnd();
}

void CValtan_BT_Attack_GroggyBall::On_FirstAnimStart()
{
}

void CValtan_BT_Attack_GroggyBall::On_LastAnimEnd()
{
}



CValtan_BT_Attack_GroggyBall* CValtan_BT_Attack_GroggyBall::Create(void* pArg)
{
	CValtan_BT_Attack_GroggyBall* pInstance = new CValtan_BT_Attack_GroggyBall;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_GroggyBall::Free()
{
	__super::Free();
}
