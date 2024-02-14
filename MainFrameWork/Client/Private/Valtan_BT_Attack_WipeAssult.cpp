#include "stdafx.h"
#include "Valtan_BT_Attack_WipeAssult.h"
#include "Monster.h"
#include "Model.h"
#include "Transform.h"
#include <Boss_Valtan.h>
#include "GameInstance.h"
#include <Skill.h>
#include "ColliderSphere.h"
#include "StaticModel.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Effect.h"
#include "Effect_Manager.h"

CValtan_BT_Attack_WipeAssult::CValtan_BT_Attack_WipeAssult()
{
}

void CValtan_BT_Attack_WipeAssult::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;

	m_bTrailEffect[0] = false;
	m_bTrailEffect[1] = false;

	m_bWarningEffect[0] = false;
	m_bWarningEffect[1] = false;

	m_fLightningTime = 0.05f;
	m_fLightningAcc = 0.0f;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_WipeAssult::OnUpdate(const _float& fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() ==m_vecAnimDesc[0].iAnimIndex)
		static_cast<CMonster*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

	if (m_bWarningEffect[0] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_WipeWarning", &tDesc);

		m_bWarningEffect[0] = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		Update_LightningEffect(fTimeDelta);
	}

	if (m_bTrailEffect[0] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 5)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_WipeTrail", &tDesc);

		m_bTrailEffect[0] = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[2].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[2].iAnimIndex) >= 7 && m_bShoot[0])
	{
		m_bShoot[0] = false;	
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#262 (50890152)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(0.5f, 50.0f, 0.1f, 10.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		vPos += vLook * 3.5f;
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(5.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(99999);
			static_cast<CSkill*>(pSkill)->Set_Force(0.f);
			static_cast<CSkill*>(pSkill)->Set_Destructive(true);
		}

		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		matWorld.Translation(vPos);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"VT_Wipe1", &tDesc);

		for (size_t i = 0; i < 6; i++)
		{
			pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
			if (pSkill != nullptr)
			{
				pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
				pSkill->Get_TransformCom()->LookAt_Dir(vLook);
				pSkill->Get_TransformCom()->My_Rotation(Vec3(0.f, 60.f * (_float)i, 0.f));
				static_cast<CSkill*>(pSkill)->Set_Atk(99999);
				static_cast<CSkill*>(pSkill)->Set_Force(0.f);
				static_cast<CSkill*>(pSkill)->Set_Destructive(true);
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START(L"VT_Wipe2", &tDesc);

			matWorld *= Matrix::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(60.0f));
			matWorld.Translation(vPos);
		}
	}

	if (m_bWarningEffect[1] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_WipeWarning2", &tDesc);

		m_bWarningEffect[1] = true;
	}

	if (m_bTrailEffect[1] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 6)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_WipeTrail", &tDesc);

		m_bTrailEffect[1] = true;
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[5].iAnimIndex) >= 8&&	m_bShoot[1])
	{
		m_bShoot[1] = false;		
		Add_Sound(L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#242 (493658097)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(1.5f, 120.0f, 1.8f, 12.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		vPos += vLook * 3.f;
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_SphereInstant", &ModelDesc);
		if (pSkill != nullptr)
		{
			pSkill->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, vPos);
			pSkill->Get_TransformCom()->LookAt_Dir(-vLook);
			pSkill->Get_Colider(_uint(LAYER_COLLIDER::LAYER_SKILL_BOSS))->Set_Radius(30.f);
			static_cast<CSkill*>(pSkill)->Set_Atk(99999);
			static_cast<CSkill*>(pSkill)->Set_Force(0.f);
			static_cast<CSkill*>(pSkill)->Set_Destructive(true);
		}
		for (auto iter : CGameInstance::GetInstance()->GetInstance()->Find_GameObjects(m_pGameObject->Get_CurrLevel(), (_uint)LAYER_TYPE::LAYER_BACKGROUND))
		{
			if (OBJ_TYPE::PROP == iter->Get_ObjectType() && iter->Get_ModelCom()->Get_ModelType() == CModel::TYPE::TYPE_NONANIM && static_cast<CStaticModel*>(iter)->Get_StaticColliders().size() != 0)
			{
				iter->Set_Active(false);
				static_cast<CStaticModel*>(iter)->Disable_NaviCells();
			}
		}

		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		matWorld.Translation(vPos);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"VT_Wipe3", &tDesc);
	}
	//sound
	{
		Add_Sound(0, 0, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#360 (396911040)");
		Add_Sound(1, 1, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2 [146]");
		Add_Sound(4, 2, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#14 (1063317697)");
		Add_Sound(5, 3, L"Effect", L"WWISEDEFAULTBANK_S_MOB_G_VOLTAN2#196 (744205178)");
	}

	return __super::OnUpdate(fTimeDelta);
}

void CValtan_BT_Attack_WipeAssult::OnEnd()
{
	__super::OnEnd();
	static_cast<CBoss_Valtan*>(m_pGameObject)->Reserve_WeaponAnimation(L"att_battle_8_01_loop", 0.2f, 0, 0, 1.15f);
}

void CValtan_BT_Attack_WipeAssult::Update_LightningEffect(_float fTimeDelta)
{
	/*Matrix matPivot = XMMatrix
	for (auto& iter = m_Effects.begin(); iter != m_Effects.end();)
	{
		if ((*iter)->Is_Active() == false)
		{
			iter = m_Effects.erase(iter);
		}
		else
		{
			(*iter)
		}
	}*/

	/*m_fLightningAcc += fTimeDelta;
	if (m_fLightningAcc >= m_fLightningTime)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		Matrix matWorld = Matrix::CreateScale(CGameInstance::GetInstance()->Random_Float(1.0f, 2.0f));
		matWorld.Translation(static_cast<CBoss_Valtan*>(m_pGameObject)->Get_Weapon()->Get_Part_WorldMatrix().Translation());
		tDesc.pPivotMatrix = &matWorld;

		wstring szEffectName = L"VT_WipeLightning";
		szEffectName += to_wstring(rand() % 4 + 1);
		EFFECT_START(szEffectName, &tDesc);
		m_fLightningAcc = 0.0f;
	}*/
}



CValtan_BT_Attack_WipeAssult* CValtan_BT_Attack_WipeAssult::Create(void* pArg)
{
	CValtan_BT_Attack_WipeAssult* pInstance = new CValtan_BT_Attack_WipeAssult;
	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CValtan_BT_Attack_WipeAssult");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CValtan_BT_Attack_WipeAssult::Free()
{
	__super::Free();
}
