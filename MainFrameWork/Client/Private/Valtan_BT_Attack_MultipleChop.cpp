#include "stdafx.h"
#include "Valtan_BT_Attack_MultipleChop.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "Effect_Manager.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "ColliderSphere.h"

CValtan_BT_Attack_MultipleChop::CValtan_BT_Attack_MultipleChop()
{
}

void CValtan_BT_Attack_MultipleChop::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_MultipleChop::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) > 55 && m_iCurrAnimation == 0)
	{
		m_iCurrAnimation = 1;

		m_pGameObject->Get_ModelCom()->Reserve_NextAnimation(m_vecAnimDesc[1].iAnimIndex, m_vecAnimDesc[1].fChangeTime,
			m_vecAnimDesc[1].iStartFrame, m_vecAnimDesc[1].iChangeFrame, m_vecAnimDesc[1].fRootDist, m_vecAnimDesc[1].IsRootRot);
	}
	if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iCurrAnimation].iAnimIndex) < 35&& m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iCurrAnimation].iAnimIndex && m_iCurrAnimation != m_iMaxAnimation - 2)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 50 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#39 (485047580)");

		_uint iAnimFrame0 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex);

		if (false == m_bEffectStart[0] && 50 <= iAnimFrame0 && m_vecAnimDesc[0].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_MultipleChop_1st", &tDesc);

			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_RadialBlur(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.05f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.08f, 100.f, 0.2f, 5.0f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Chromatic(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.05f);

			m_bEffectStart[0] = true;
		}

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 30 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#40 (235213962)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		_uint iAnimFrame1 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex);

		if (false == m_bEffectStart[1] && 30 <= iAnimFrame1 && m_vecAnimDesc[1].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_MultipleChop_2nd", &tDesc);

			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_RadialBlur(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.075f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.1f, 100.f, 0.2f, 5.0f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Chromatic(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.075f);

			m_bEffectStart[1] = true;
		}

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS));
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
		}
	}
	if (m_iCurrAnimation == 2 && m_vecAnimDesc[1].iAnimIndex == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 30 && m_bShoot[2])
	{
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#41 (667792344)");
		m_bShoot[2] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		_uint iAnimFrame2 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex);

		if (false == m_bEffectStart[2] && m_vecAnimDesc[1].iAnimIndex == m_vecAnimDesc[2].iAnimIndex && 30 <= iAnimFrame2 && m_vecAnimDesc[2].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_MultipleChop_3rd", &tDesc);

			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_RadialBlur(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.075f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.12f, 100.f, 0.2f, 5.0f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Chromatic(0.05f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.075f);

			m_bEffectStart[2] = true;
		}

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS));
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.f);
		}
	}
	if (m_iCurrAnimation == m_iMaxAnimation - 2 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[m_iMaxAnimation-2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[m_iMaxAnimation - 2].iAnimIndex) >= 37 && m_bShoot[3])
	{
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#42 (236377491)");
		m_bShoot[3] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;

		_uint iAnimFrame3 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex);
		if (false == m_bEffectStart[3] && 37 <= iAnimFrame3 && m_vecAnimDesc[3].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_MultipleChop_4th", &tDesc);

			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_RadialBlur(0.07f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.12f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.12f, 100.f, 0.4f, 5.0f);
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Chromatic(0.07f, tDesc.pPivotMatrix->Translation(), 0.2f, 0.12f);

			m_bEffectStart[3] = true;
		}

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 3.f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.5f);
		}
		if (static_cast<CBoss*>(m_pGameObject)->Get_Phase() == 2)
		{
			_float	fDegree = 0.f;
			for (size_t i = 0; i < 6; i++)
			{
				CSkill::ModelDesc ModelDesc = {};
				ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
				ModelDesc.iObjectID = -1;
				ModelDesc.pOwner = m_pGameObject;
				fDegree = i * 60.f;

				CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereTerm", &ModelDesc);
				if (pSkill != nullptr)
				{
					Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
					Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
					vLook.Normalize();
					vPos += vLook * 3.f;
					vLook =  Vec3::TransformNormal(vLook, XMMatrixRotationY(XMConvertToRadians(fDegree)));
					vLook.Normalize();
					vPos += vLook * 4.f;
					pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
					pSkill->Get_TransformCom()->LookAt_Dir(vLook);
					pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(2.5f);
					static_cast<CSkill*>(pSkill)->Set_SoundTag(L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#249 (953298922).wav"); 
				}
			}
		}
	}

	//sound
	{
		Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#113 (872104708)",29);
		Add_Sound(1, 1, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#44 (813796387)", 25);
		if(m_iCurrAnimation == 2 && m_vecAnimDesc[1].iAnimIndex == m_vecAnimDesc[2].iAnimIndex)
			Add_Sound(1, 2, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#45 (674429565)",25);
		Add_Sound(m_iMaxAnimation - 2, 3, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#43 (1015057785)",30);
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_MultipleChop::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}

void CValtan_BT_Attack_MultipleChop::On_FirstAnimStart()
{
	__super::On_FirstAnimStart();

	for (_int i = 0; i < 4; ++i)
		m_bEffectStart[i] = false;
}

CValtan_BT_Attack_MultipleChop* CValtan_BT_Attack_MultipleChop::Create(void* pArg)
{
	CValtan_BT_Attack_MultipleChop* pInstance = new CValtan_BT_Attack_MultipleChop;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_MultipleChop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_MultipleChop::Free()
{
	__super::Free();
}
