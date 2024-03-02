#include "stdafx.h"
#include "Valtan_BT_Attack_GhostGrab.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include "ColliderSphere.h"
#include <Skill.h>
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect_Manager.h"
#include <Boss_Valtan.h>

CValtan_BT_Attack_GhostGrab::CValtan_BT_Attack_GhostGrab()
{
}

void CValtan_BT_Attack_GhostGrab::OnStart()
{
	__super::OnStart();
	m_bShoot = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_GhostGrab::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 42)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Radius(2.f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_Offset(Vec3(0.f, 1.3f, -1.1f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand")));
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[0].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[0].iAnimIndex) >= 52)
	{
		CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Valtan#28 (615573039)");
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_GRAB_BOSS)->SetActive(false);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 19 && m_bShoot)
	{
		m_bShoot = false;
		CSound_Manager::GetInstance()->Stop_Channel_Sound(L"Valtan#217 (1053313532)");
		Add_Sound(L"Effect", L"Valtan#48 (503572100)");
		Add_Sound(L"Effect", L"Valtan#50 (428724023)");
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;


		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_Breath", &ModelDesc);
		if (pSkill != nullptr)
		{
			Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
			vLook.Normalize();
			vPos -= vLook * 0.2f;
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		}
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(0.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.f, 1.3f, -1.1f));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(100);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(49.9f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand")));
	}

	Add_Sound_Channel(0, 0, L"Effect", L"Valtan#28 (615573039)");
	Add_Sound_Channel(2, 1, L"Effect", L"Valtan#217 (1053313532)");

	Update_Effect();

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_GhostGrab::OnEnd()
{
	__super::OnEnd();
	m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	CSkill::ModelDesc ModelDesc = {};
	ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
	ModelDesc.iObjectID = -1;
	ModelDesc.pOwner = m_pGameObject;
	CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
	if (pSkill != nullptr)
	{
		_uint iBoneIndex = m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand"));
		Matrix Pivot = m_pGameObject->Get_ModelCom()->Get_PivotMatrix();
		XMMATRIX BoneMatrix = m_pGameObject->Get_ModelCom()->Get_CombinedMatrix(iBoneIndex) * Pivot;

		BoneMatrix.r[0] = XMVector3Normalize(BoneMatrix.r[0]);
		BoneMatrix.r[1] = XMVector3Normalize(BoneMatrix.r[1]);
		BoneMatrix.r[2] = XMVector3Normalize(BoneMatrix.r[2]);

		BoneMatrix *= m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Matrix matWorld = BoneMatrix;
		Vec3 vPos = matWorld.Translation();
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(0.5f);
		pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
		pSkill->Get_TransformCom()->LookAt_Dir(vLook);
		static_cast<CSkill*>(pSkill)->Set_Atk(0);
		static_cast<CSkill*>(pSkill)->Set_Force(20.f);
	}
}

void CValtan_BT_Attack_GhostGrab::On_FirstAnimStart()
{
	__super::On_FirstAnimStart();

	for (_int i = 0; i < 2; ++i)
		m_bEffectStart[i] = false;
}

void CValtan_BT_Attack_GhostGrab::Update_Effect()
{
	_uint iAnimFrame0 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex);
	_uint iAnimFrame1 = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex);

	if (false == m_bEffectStart[0] && m_vecAnimDesc[2].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_Grab_WarningPizza", &tDesc);

		m_bEffectStart[0] = true;
	}

	if (false == m_bEffectStart[1] && iAnimFrame1 >= 20 && m_vecAnimDesc[3].iAnimIndex == m_pGameObject->Get_ModelCom()->Get_CurrAnim())
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_Grab_Scream", &tDesc);

		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_RadialBlur(0.2f, tDesc.pPivotMatrix->Translation(), 0.15f, 0.1f);
		m_bEffectStart[1] = true;
	}
}

CValtan_BT_Attack_GhostGrab* CValtan_BT_Attack_GhostGrab::Create(void* pArg)
{
	CValtan_BT_Attack_GhostGrab* pInstance = new CValtan_BT_Attack_GhostGrab;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_GhostGrab");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_GhostGrab::Free()
{
	__super::Free();
}
