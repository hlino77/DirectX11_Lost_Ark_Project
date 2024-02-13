#include "stdafx.h"
#include "Valtan_BT_Attack_RushGrab.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"

CValtan_BT_Attack_RushGrab::CValtan_BT_Attack_RushGrab()
{
}

void CValtan_BT_Attack_RushGrab::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_RushGrab::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex&& m_fLoopTime < m_vecAnimDesc[m_iCurrAnimation].fMaxLoopTime - 0.5f)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && !m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Radius(1.4f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->IsNext())
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(false);
		m_pGameObject->Get_TransformCom()->Turn_Speed(m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_UP), XMConvertToRadians(280.f), fTimeDelta);
		Add_Sound(3, 2, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#299 (954830834)");
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 20 && m_bShoot)
	{
		m_bShoot = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		CSound_Manager::GetInstance()->Stop_Channel_Sound(L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#217 (1053313532)");
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#196 (744205178)");
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#50 (428724023)");

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_Breath", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}
	}
	//sound
	{
		Add_Sound(1, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#292 (957066089)");
		Add_Sound(3, 1, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#288 (673586812)");
		Add_Sound_Channel(5, 3, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#217 (1053313532)");
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_RushGrab::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}



CValtan_BT_Attack_RushGrab* CValtan_BT_Attack_RushGrab::Create(void* pArg)
{
	CValtan_BT_Attack_RushGrab* pInstance = new CValtan_BT_Attack_RushGrab;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_RushGrab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_RushGrab::Free()
{
	__super::Free();
}
