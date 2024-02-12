#include "stdafx.h"
#include "Valtan_BT_Attack_Imposter.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss.h>
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include <Skill_Crystal.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"

CValtan_BT_Attack_Imposter::CValtan_BT_Attack_Imposter()
{
}

void CValtan_BT_Attack_Imposter::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Imposter::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		m_pGameObject->Set_Render(false);
	}
	if (m_iCurrAnimation == 2)
		m_pGameObject->Set_Render(true);
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 48)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(0.1f, 0.6f);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
	}
	if (m_iCurrAnimation == 4 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 108)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Is_SetuponCell() == true)
		{
			static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(false);
			static_cast<CBoss*>(m_pGameObject)->Set_Weapon_Render(false);
		}
		if(m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 112)
			m_pGameObject->Get_TransformCom()->Go_Up(fTimeDelta*6.f);
		else
			m_pGameObject->Get_TransformCom()->Go_Up(fTimeDelta*2.f);
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_fLoopTime < 3.f)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Is_SetuponCell() == false)
			static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(true);
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		m_pGameObject->Set_Render(false);
	}
	if (m_iCurrAnimation == 6)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_Weapon_Render(true);
		m_pGameObject->Set_Render(true);
	}
	if (m_iCurrAnimation == 9 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[9].iAnimIndex && m_fLoopTime < 3.f)
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
	

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 73 && m_bShoot[0])
	{
		m_bShoot[0] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.5f, 50.0f, 0.1f, 10.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		vPos += vLook * 2.f;
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(3.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(40);
			static_cast<CSkill*>(pSkill)->Set_Force(32.f);
		}

		for (size_t i = 0; i < 6; i++)
		{
			vLook.Normalize();
			pSkill = nullptr;
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_TransformCom()->My_Rotation(Vec3(0.f, 60.f * (_float)i, 0.f));
				static_cast<CSkill*>(pSkill)->Set_Atk(40);
				static_cast<CSkill*>(pSkill)->Set_Force(32.f);
				static_cast<CSkill*>(pSkill)->Set_PizzaSlope(15.f,-15.f);
			}
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[3].iAnimIndex) >= 73 && m_bShoot[3])
	{
		m_bShoot[3] = false;
		Vec3 vDir = Vec3(1.f, 0.f, 1.f);
		vDir.Normalize();
		Matrix matRotation90 = Matrix::CreateRotationY(XMConvertToRadians(90.f));
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.5f, 50.0f, 0.1f, 10.0f);
		for (size_t i = 0; i < 4; i++)
		{
			Vec3 vPosition = static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition();
			vDir = Vec3::TransformNormal(vDir, matRotation90);
			vDir.y = 0.f;
			vDir.Normalize();
			vPosition += vDir * 10.f;
			vPosition.y = -60.f;
			Add_Stone(vPosition);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 8 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(2.f, 110.0f, 1.5f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		CGameObject* pSkill =  CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_Atk(99999);
			static_cast<CSkill*>(pSkill)->Set_Force(35.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(30.f, -30.f);
			static_cast<CSkill*>(pSkill)->Set_SafeZonePierce(true);
			static_cast<CSkill*>(pSkill)->Set_Destructive(true);
		}
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(30.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(450);
			static_cast<CSkill*>(pSkill)->Set_Force(33.f);
		}
	
	}
	
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[10].iAnimIndex) >= 3 && m_bShoot[2])
	{
		m_bShoot[2] = false;
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_AxeWave", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			static_cast<CSkill*>(pSkill)->Set_Atk(550);
			static_cast<CSkill*>(pSkill)->Set_Force(51.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(20.f, -20.f);
			static_cast<CSkill*>(pSkill)->Set_SafeZonePierce(true);
			static_cast<CSkill*>(pSkill)->Set_Destructive(true);
		}
	}
	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_Imposter::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
	for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL))
	{
		if (pGameObject->Get_ObjectTag() == L"Skill_Crystal")
		{
			dynamic_cast<CSkill_Crystal*>(pGameObject)->Set_Explosion(true);
		}
	}
}




CValtan_BT_Attack_Imposter* CValtan_BT_Attack_Imposter::Create(void* pArg)
{
	CValtan_BT_Attack_Imposter* pInstance = new CValtan_BT_Attack_Imposter;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_Imposter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_Imposter::Free()
{
	__super::Free();
}
