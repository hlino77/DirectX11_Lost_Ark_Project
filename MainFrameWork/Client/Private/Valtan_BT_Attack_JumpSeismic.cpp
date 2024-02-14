#include "stdafx.h"
#include "Valtan_BT_Attack_JumpSeismic.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect.h"
#include "Effect_Manager.h"

CValtan_BT_Attack_JumpSeismic::CValtan_BT_Attack_JumpSeismic()
{
}

void CValtan_BT_Attack_JumpSeismic::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
	m_bWhirlWind = false;
	m_fAttackAcc = 0.f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_JumpSeismic::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[2].iAnimIndex)
	{ 
		if(static_cast<CBoss*>(m_pGameObject)->Get_Target_Distance()>0.2f)
			static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction();
		static_cast<CBoss*>(m_pGameObject)->Move_Dir(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK),
			0.7f * static_cast<CBoss*>(m_pGameObject)->Get_MoveSpeed(), fTimeDelta);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 37 && m_bShoot)
	{
		m_bShoot = false;
		Add_Sound(L"Effect",L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#221 (561982981)");
		Add_Sound(L"Effect",L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#220 (262642769)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.15f, 50.0f, 0.3f, 10.0f);
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SeismicWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos += vLook * 2.5f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#220 (262642769)");
		}

		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			Vec3 vLook = matWorld.Backward();
			Vec3 vPos = matWorld.Translation();
			vPos += vLook * 1.5f;
			matWorld.Translation(vPos);
			Matrix matRot = Matrix::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XM_PI * 0.5f);
			for (_uint i = 0; i < 4; ++i)
			{
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &matWorld;
				EFFECT_START(L"VTSeismic", &tDesc);

				matWorld *= matRot;
				matWorld.Translation(vPos);
			}
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex)
	{
		if (m_bWhirlWind == true)
		{
			m_pWhirlWind->Update_Pivot(m_pGameObject->Get_TransformCom()->Get_WorldMatrix());
		}
		else
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(3.5f);
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
			static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
			static_cast<CBoss*>(m_pGameObject)->Set_Force(15.f);

			vector<CEffect*> Effects;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"VTSeismicWhirlWindTrail", &tDesc, Effects);
			m_pWhirlWind = Effects.front();

			m_bWhirlWind = true;
		}
		if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) > m_pGameObject->Get_ModelCom()->Get_Anim_MaxFrame(m_vecAnimDesc[2].iAnimIndex)-3)
		{
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive());
		}
		m_fAttackAcc += fTimeDelta;
		if (m_fAttackAcc > 0.2f)
		{
			m_fAttackAcc = 0.f;
			m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(!m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->IsActive());
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 12)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	}	
	{
		Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#78 (647906992)");
		Add_Sound(1, 1, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#88 (84610412)");
		Add_Sound(2, 2, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#222 (193078861)");
		Add_Sound(3, 3, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#223 (403055014)");
		Add_Sound(3, 4, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#113 (872104708)");
	}
		
		
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_JumpSeismic::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_JumpSeismic* CValtan_BT_Attack_JumpSeismic::Create(void* pArg)
{
	CValtan_BT_Attack_JumpSeismic* pInstance = new CValtan_BT_Attack_JumpSeismic;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_JumpSeismic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_JumpSeismic::Free()
{
	__super::Free();
}
