#include "stdafx.h"
#include "Valtan_BT_Attack_FirstTerrainDestruction.h"
#include <Boss_Valtan.h>
#include "Model.h"
#include "Transform.h"
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include <Skill_Crystal.h>
#include <AnimModel.h>
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"

CValtan_BT_Attack_FirstTerrainDestruction::CValtan_BT_Attack_FirstTerrainDestruction()
{
}

void CValtan_BT_Attack_FirstTerrainDestruction::OnStart()
{
	__super::OnStart();
	m_vLandPosition = Vec3(104.5f, 0.f, 104.5f);
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_FirstTerrainDestruction::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_Dir(XMVector3Normalize(m_vLandPosition - static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition()));
		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vLandPosition);
	}

	if ( m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		m_pGameObject->Get_TransformCom()->LookAt_ForLandObject(static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition());
		m_pGameObject->Get_TransformCom()->Go_Straight(static_cast<CBoss*>(m_pGameObject)->Get_MoveSpeed() * 1.1f, fTimeDelta);
	}

	if (m_iCurrAnimation == 12 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[12].iAnimIndex && m_fLoopTime < 3.f)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 9 && m_bShoot[0])
	{
		m_bShoot[0] = false;	
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(1.1f, 150.0f, 1.5f, 13.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition();

		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_ForLandObject(m_vLandPosition);
			pSkill->Get_TransformCom()->My_Rotation(Vec3(0.f, 57.f, 0.f));
			static_cast<CSkill*>(pSkill)->Set_Atk(40);
			static_cast<CSkill*>(pSkill)->Set_Force(32.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(55.f, -45.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 5.f);
		}
		pSkill = nullptr;
		pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_ForLandObject(m_vLandPosition);
			pSkill->Get_TransformCom()->My_Rotation(Vec3(0.f, -33.f, 0.f));
			static_cast<CSkill*>(pSkill)->Set_Atk(40);
			static_cast<CSkill*>(pSkill)->Set_Force(32.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(45.f, -55.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaRadii(30.f, 5.f);
		}
		{
			CGameObject* pFloor = CGameInstance::GetInstance()->GetInstance()->Find_GameObject(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_BACKGROUND, L"Floor_All_L01");
			if (pFloor != nullptr && pFloor->Get_ModelCom()->Get_ModelType() == CModel::TYPE::TYPE_ANIM &&	pFloor->Get_ModelName() == TEXT("Floor_All_L01"))
			{
				static_cast<CAnimModel*>(pFloor)->Break_Floor();
			}
		}
	}


	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(true);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Radius(3.5f);
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_Offset(Vec3(0.46f, 0.f, -1.65f));
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->Set_BoneIndex(m_pGameObject->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-spine")));
		static_cast<CBoss*>(m_pGameObject)->Set_Atk(30);
		static_cast<CBoss*>(m_pGameObject)->Set_Force(45.f);
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 12)
		m_pGameObject->Get_Colider((_uint)LAYER_COLLIDER::LAYER_ATTACK_BOSS)->SetActive(false);
	
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 22 && m_bShoot[3])
	{
		m_bShoot[3] = false;
		Vec3 vDir = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);;
		vDir.Normalize();
		Matrix matRotation90 = Matrix::CreateRotationY(XMConvertToRadians(90.f));

		for (size_t i = 0; i < 4; i++)
		{
			Vec3 vPosition = static_cast<CBoss*>(m_pGameObject)->Get_SpawnPosition();
			vDir = Vec3::TransformNormal(vDir, matRotation90);
			vDir.y = 0.f;
			vDir.Normalize();
			vPosition += vDir * 5.f;
			vPosition.y = -10.f;
			Add_Stone(vPosition);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[10].iAnimIndex) >= 22 && m_bShoot[1])
	{
		m_bShoot[1] = false;		
		static_cast<CBoss*>(m_pGameObject)->Set_RimLight(1.f, 0.7f);
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(1.f, 0.7f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(30.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(50);
			static_cast<CSkill*>(pSkill)->Set_Force(32.f);
		}
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[13].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[13].iAnimIndex) >= 15 && m_bShoot[2])
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
			static_cast<CSkill*>(pSkill)->Set_Atk(50);
			static_cast<CSkill*>(pSkill)->Set_Force(51.f);
			static_cast<CSkill*>(pSkill)->Set_PizzaSlope(20.f, -20.f);
			static_cast<CSkill*>(pSkill)->Set_Destructive(true);
			static_cast<CSkill*>(pSkill)->Set_SafeZonePierce(true);
		}
	}


	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_FirstTerrainDestruction::OnEnd()
{
	__super::OnEnd();
	for (auto pGameObject : CGameInstance::GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_SKILL))
	{
		if (pGameObject->Get_ObjectTag() == L"Skill_Crystal")
		{
			dynamic_cast<CSkill_Crystal*>(pGameObject)->Set_Explosion(true);
		}
	}
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}


CValtan_BT_Attack_FirstTerrainDestruction* CValtan_BT_Attack_FirstTerrainDestruction::Create(void* pArg)
{
	CValtan_BT_Attack_FirstTerrainDestruction* pInstance = new CValtan_BT_Attack_FirstTerrainDestruction;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_FirstTerrainDestruction");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_FirstTerrainDestruction::Free()
{
	__super::Free();
}
