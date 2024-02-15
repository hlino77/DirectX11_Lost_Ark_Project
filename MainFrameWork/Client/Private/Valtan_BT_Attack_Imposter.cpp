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
#include "Effect_Manager.h"
#include "Effect.h"
#include "AsUtils.h"


CValtan_BT_Attack_Imposter::CValtan_BT_Attack_Imposter()
{
	m_vTargetPos = Vec3(100.0f, 0.19f, 100.0f);
}

void CValtan_BT_Attack_Imposter::OnStart()
{
	__super::OnStart();
	m_bShoot[0] = true;
	m_bShoot[1] = true;
	m_bShoot[2] = true;
	m_bShoot[3] = true;

	m_bWarning[0] = false;
	m_bShoutEffect = false;
	m_bJumpEffect = false;

	m_iCameraSequence = 0;
	m_WarningEffect1.clear();

	m_bLastTrail = false;
	m_bLastAttack = false;

	m_pLastWarning = nullptr;
}

CBT_Node::BT_RETURN CValtan_BT_Attack_Imposter::OnUpdate(const _float& fTimeDelta)
{
	Update_Camera(fTimeDelta);

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[1].iAnimIndex)
	{
		static_cast<CBoss*>(m_pGameObject)->Move_to_SpawnPosition();
		m_pGameObject->Get_TransformCom()->LookAt_Dir(Vec3(0.f, 0.f, -1.f));
		m_pGameObject->Set_Render(false);
	}
	if (m_iCurrAnimation == 2)
		m_pGameObject->Set_Render(true);
	if (m_bShoutEffect == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 43)
	{
		static_cast<CBoss_Valtan*>(m_pGameObject)->Set_Weapon_RimLight(0.1f, 0.7f);
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ImpShouting", &tDesc);

		m_bShoutEffect = true;
	}
	if (m_iCurrAnimation == 4 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[4].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 106)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Is_SetuponCell() == true)
		{
			static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(false);
			static_cast<CBoss*>(m_pGameObject)->Set_Weapon_Render(false);
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[4].iAnimIndex, 1.15f);
		}
		if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 112)
			m_pGameObject->Get_TransformCom()->Go_Up(fTimeDelta * 6.f);
		else
			m_pGameObject->Get_TransformCom()->Go_Up(fTimeDelta * 2.f);

		if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 130 && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) < 140)
		{
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[4].iAnimIndex, 0.2f);
		}
		else if (m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex) >= 140)
		{
			m_pGameObject->Get_ModelCom()->Set_Anim_Speed(m_vecAnimDesc[4].iAnimIndex, 1.15f);
		}
		if (m_bJumpEffect == false)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ImpJump", &tDesc);

			m_bJumpEffect = true;
		}
	}
	if (m_iCurrAnimation == 5 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[5].iAnimIndex && m_fLoopTime < 3.f)
	{
		if (static_cast<CBoss*>(m_pGameObject)->Is_SetuponCell() == false)
			static_cast<CBoss*>(m_pGameObject)->Set_SetuponCell(true);

		m_pGameObject->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, m_vTargetPos);
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);
		m_pGameObject->Set_Render(false);

		if (m_WarningEffect1.empty())
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			matWorld.Translation(m_vTargetPos);

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;

			EFFECT_START_OUTLIST(L"VT_ImpWarning2", &tDesc, m_WarningEffect1);
		}
		else
		{
			Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			matWorld.Translation(m_vTargetPos);
			for (auto& Effect : m_WarningEffect1)
			{
				Effect->Update_Pivot(matWorld);
			}
		}
	}
	if (m_iCurrAnimation == 6)
	{
		static_cast<CBoss*>(m_pGameObject)->Set_Weapon_Render(true);
		m_pGameObject->Set_Render(true);
	}
	if (m_iCurrAnimation == 9 && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[9].iAnimIndex && m_fLoopTime < 3.f)
	{
		static_cast<CBoss*>(m_pGameObject)->LookAt_Target_Direction_Lerp(fTimeDelta);

		if (m_pLastWarning == nullptr)
		{
			vector<CEffect*> Effects;
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START_OUTLIST(L"VT_ImpWarning3", &tDesc, Effects);
			m_pLastWarning = Effects.front();
		}
		else
		{
			m_pLastWarning->Update_Pivot(m_pGameObject->Get_TransformCom()->Get_WorldMatrix());
		}
	}


	if (m_bWarning[0] == false && m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[3].iAnimIndex)
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ImpWarning", &tDesc);
		m_bWarning[0] = true;
	}


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
			static_cast<CSkill*>(pSkill)->Set_SoundTag(L"Valtan#160 (275687761)");
		}

		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vEffectPos = vPos;
		vPos.x -= 0.3f;
		vPos.z -= 2.0f;

		matWorld.Translation(vEffectPos);
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START(L"VT_ImpStart1", &tDesc);
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
				static_cast<CSkill*>(pSkill)->Set_PizzaSlope(15.f, -15.f);
			}

			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &matWorld;
			EFFECT_START(L"VT_ImpStart2", &tDesc);

			matWorld *= Matrix::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(60.0f));
			matWorld.Translation(vEffectPos);
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

			{
				Matrix matWorld;
				matWorld.Translation(vPosition);
				CEffect_Manager::EFFECTPIVOTDESC tDesc;
				tDesc.pPivotMatrix = &matWorld;
				EFFECT_START(L"VT_FTStoneSpawn", &tDesc);
			}

			vPosition.y = -3.f;
			Add_Stone(vPosition);
		}
	}
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[6].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[6].iAnimIndex) >= 8 && m_bShoot[1])
	{
		m_bShoot[1] = false;
		Add_Sound(L"Effect", L"Valtan#160 (275687761)");
		Add_Sound(L"Effect", L"Valtan#375 (293240220)");
		CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Cam_Shake(2.f, 110.0f, 1.5f, 11.0f);
		CSkill::ModelDesc ModelDesc = {};
		ModelDesc.iLayer = (_uint)LAYER_TYPE::LAYER_SKILL;
		ModelDesc.iObjectID = -1;
		ModelDesc.pOwner = m_pGameObject;
		Vec3 vPos = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vLook = m_pGameObject->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
		vLook.Normalize();
		CGameObject* pSkill = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Skill_Valtan_PizzaInstant", &ModelDesc);
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

		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		EFFECT_START(L"VT_ImpLand", &tDesc);
	}

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[10].iAnimIndex) >= 5 && m_bShoot[2])
	{
		m_bShoot[2] = false;
		Add_Sound(L"Effect", L"Valtan#225 (910655979)");
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

	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() == m_vecAnimDesc[10].iAnimIndex)
	{
		if (m_pLastWarning != nullptr)
		{
			m_pLastWarning->EffectEnd();
			m_pLastWarning = nullptr;
		}

		if (m_bLastTrail == false && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[10].iAnimIndex) >= 5)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ImpTrail1", &tDesc);
			m_bLastTrail = true;
		}

		if (m_bLastAttack == false && m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[10].iAnimIndex) >= 6)
		{
			CEffect_Manager::EFFECTPIVOTDESC tDesc;
			tDesc.pPivotMatrix = &m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
			EFFECT_START(L"VT_ImpTrail2", &tDesc);
			m_bLastAttack = true;
		}
	}

	//sound
	{
		Add_Sound(0, 0, L"Effect", L"Valtan#113 (872104708)");
		Add_Sound(0, 1, L"Effect", L"Valtan#153 (153317365)", 24);
		Add_Sound(2, 2, L"Effect", L"Valtan#43 (1015057785)", 7);
		Add_Sound(2, 3, L"Effect", L"Valtan#42 (236377491)", 7);
		Add_Sound(3, 4, L"Effect", L"Valtan#150 (508288221)");
		Add_Sound(3, 5, L"Effect", L"Valtan#59 (137026933)");
		Add_Sound(3, 6, L"Effect", L"Valtan#14 (1063317697)", 30);
		Add_Sound(3, 7, L"Effect", L"Valtan#388 (332056691)", 30);
		Add_Sound(4, 8, L"Effect", L"Valtan#281#Revised");
		Add_Sound(4, 9, L"Effect", L"Valtan#161 (23475606)", 43);
		Add_Sound(4, 10, L"Effect", L"Valtan#401 (669702991)", 90);
		Add_Sound(4, 11, L"Effect", L"Valtan#50 (428724023)", 108);
		Add_Sound(5, 12, L"Effect", L"Valtan#110 (618835666)");
		Add_Sound(8, 13, L"Effect", L"Valtan#15 (848266953)");
		Add_Sound(8, 14, L"Effect", L"Valtan#84 (271442095)", 27);
		Add_Sound(9, 15, L"Effect", L"Valtan [146]");
		Add_Sound(10, 16, L"Effect", L"Valtan#110 (618835666)");
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

void CValtan_BT_Attack_Imposter::Update_Camera(_float fTimeDelta)
{
	if (m_pGameObject->Get_ModelCom()->Get_CurrAnim() != m_vecAnimDesc[4].iAnimIndex)
	{
		return;
	}

	CCamera_Player* pCamera = CServerSessionManager::GetInstance()->Get_Player()->Get_Camera();
	_uint iAnimFrame = m_pGameObject->Get_ModelCom()->Get_Anim_Frame(m_vecAnimDesc[4].iAnimIndex);

	if (m_iCameraSequence == 0)
	{
		pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vTargetPos = matWorld.Translation();
		vTargetPos.y += 2.0f;
		vTargetPos += matWorld.Right() * 0.5f;
		
		Vec3 vOffset = (matWorld.Backward() * 0.5f) + matWorld.Right() - (matWorld.Up() * 0.2f);
		vOffset.Normalize();

		pCamera->Set_TargetPos(vTargetPos);
		pCamera->Set_Offset(vOffset);
		pCamera->Set_CameraLength(3.0f);

		m_iCameraSequence = 1;
	}
	else if (m_iCameraSequence == 1)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		Vec3 vOffset = pCamera->Get_Offset();
		Vec3 vTargetOffset = matWorld.Backward() - matWorld.Right() + (matWorld.Up() * 0.5f);
		vTargetOffset.Normalize();
		
		vOffset = Vec3::Lerp(vOffset, vTargetOffset, 1.0f * fTimeDelta);

		pCamera->Set_Offset(vOffset);

		if (iAnimFrame >= 43)
		{
			m_iCameraSequence = 2;
		}
	}
	else if (m_iCameraSequence == 2)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		Vec3 vOffset = matWorld.Backward() - (matWorld.Right() * 0.5f) + matWorld.Up()* 0.7f;
		pCamera->Set_Offset(vOffset);
		pCamera->Set_CameraLength(7.0f);
		pCamera->ZoomInOut(20.0f, 5.0f);

		pCamera->Set_RadialBlur(0.2f, matWorld.Translation(), 0.05f, 0.1f);
		m_iCameraSequence = 3;
	}
	else if (m_iCameraSequence == 3)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = XMVector3Rotate(vOffset, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), 0.5f * fTimeDelta));
		pCamera->Set_Offset(vOffset);

		if (iAnimFrame >= 81)
		{
			m_iCameraSequence = 4;
		}
	}
	else if (m_iCameraSequence == 4)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vTargetPos = matWorld.Translation();

		Vec3 vOffset = (matWorld.Backward() * 1.0f) - (matWorld.Right() * 0.1f) + (matWorld.Up() * 2.0f);
		vOffset.Normalize();

		pCamera->Set_TargetPos(vTargetPos);
		pCamera->Set_Offset(vOffset);
		pCamera->Set_CameraLength(5.0f);

		m_iCameraSequence = 5;
	}
	else if (m_iCameraSequence == 5)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		Vec3 vOffset = pCamera->Get_Offset();
		Vec3 vTargetOffset = matWorld.Backward() + matWorld.Up();
		vTargetOffset.Normalize();

		vOffset = Vec3::Lerp(vOffset, vTargetOffset, 1.0f * fTimeDelta);

		pCamera->Set_Offset(vOffset);

		if (iAnimFrame >= 108)
		{
			m_iCameraSequence = 6;
		}
	}
	else if (m_iCameraSequence == 6)
	{
		Matrix matWorld = m_pGameObject->Get_TransformCom()->Get_WorldMatrix();

		m_vCamPos = matWorld.Translation() + (matWorld.Backward() * 2.0f) + Vec3(0.0f, 0.3f, 0.0f);
		m_vCamTargetPos = matWorld.Translation();

		Vec3 vOffset = m_vCamPos - m_vCamTargetPos;
		_float fLength = vOffset.Length();
		vOffset.Normalize();
		
		pCamera->Set_Offset(vOffset);
		pCamera->Set_CameraLength(fLength);
		pCamera->Set_TargetPos(m_vCamTargetPos);

		m_iCameraSequence = 7;
	}

	if (m_iCameraSequence == 7)
	{
		m_vCamTargetPos.y = CAsUtils::Lerpf(m_vCamTargetPos.y, 20.0f, 3.0f * fTimeDelta);

		Vec3 vOffset = m_vCamPos - m_vCamTargetPos;
		_float fLength = vOffset.Length();
		vOffset.Normalize();

		pCamera->Set_Offset(vOffset);
		pCamera->Set_CameraLength(fLength);
		pCamera->Set_TargetPos(m_vCamTargetPos);

		if(iAnimFrame >= 142)
			m_iCameraSequence = 8;
	}
	else if (m_iCameraSequence == 8)
	{
		pCamera->Set_Mode(CCamera_Player::CameraState::DEFAULT);
		pCamera->Set_DefaultOffset();
		pCamera->DefaultLength(100.0f);
		m_iCameraSequence = 9;
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
