#include "stdafx.h"
#include "Valtan_BT_Attack_SwingSeismic.h"
#include "Boss_Valtan.h"
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

CValtan_BT_Attack_SwingSeismic::CValtan_BT_Attack_SwingSeismic()
{
}

void CValtan_BT_Attack_SwingSeismic::OnStart()
{
	if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		__super::OnStart(1);
	else
		__super::OnStart();
	m_bShoot = true;
	m_bSwing = false;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_SwingSeismic::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 45)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(2.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(1.42f, -0.8536f, -0.3f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("b_wp_r_01")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(15.f);
		if(!static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
			static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
	}


	if (m_bSwing == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 48)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_SeismicSwing", &tDesc);

		m_bSwing = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 50)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);

	if (!static_cast<CBoss*>(m_pGameObject)->Is_Dummy()&&m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) < 56 && m_bShoot)
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[1].iAnimIndex) >= 56 && m_bShoot)
	{
		m_bShoot = false;
		Add_Sound( L"Effect", L"Valtan#19 (579691751)");
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
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#220 (262642769)");
		}
		if (static_cast<CBoss*>(m_pGameObject)->Is_Dummy())
		{
			static_cast<CMonster*>(m_pGameObject)->Set_Die();
			static_cast<CMonster*>(m_pGameObject)->Set_AnimationSpeed(0.5f);
		}

		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = matWorld.Translation() + matWorld.Backward() * 3.5f;
		matWorld.Translation(vPos);
		
		for (_uint i = 0; i < 4; ++i)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START(L"VTSeismic", &tDesc);

			matWorld *= Matrix::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(90.0f));
			matWorld.Translation(vPos);
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

	Add_Sound(0, 0, L"Effect", L"Valtan#16 (800951587)");
	Add_Sound(0, 1, L"Effect", L"Valtan#13 (8442846)");
	Add_Sound(0, 2, L"Effect", L"Valtan#18 (376955622)",45);
	Add_Sound(1, 3, L"Effect", L"Valtan#14 (1063317697)");
	Add_Sound(1, 4, L"Effect", L"Valtan#21 (9074067)",50);

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_SwingSeismic::OnEnd()
{
	__super::OnEnd();
}



CValtan_BT_Attack_SwingSeismic* CValtan_BT_Attack_SwingSeismic::Create(void* pArg)
{
	CValtan_BT_Attack_SwingSeismic* pInstance = new CValtan_BT_Attack_SwingSeismic;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_SwingSeismic");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_SwingSeismic::Free()
{
	__super::Free();
}
