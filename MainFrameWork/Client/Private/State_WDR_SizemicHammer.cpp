#include "stdafx.h"
#include "..\Public\State_WDR_SizemicHammer.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Custom_SeismicHammerRock.h"
#include "Effect_Manager.h"
#include "Camera_Player.h"

CState_WDR_SizemicHammer::CState_WDR_SizemicHammer(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_SizemicHammer::Initialize()
{
	m_iSizemicHammer = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_seismichammer", 1.2f);
	if (m_iSizemicHammer == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_SizemicHammer::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_SizemicHammer::Tick_State_NoneControl;

	m_SkillFrames.push_back(41);
	m_SkillFrames.push_back(57);
	m_SkillFrames.push_back(-1);

	for (size_t i = 0; i < 2; i++)
	{
		m_bShake[i] = false;
	}

	return S_OK;
}

void CState_WDR_SizemicHammer::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iSizemicHammer, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_UseMarbleMessage();

	for (size_t i = 0; i < 2; i++)
	{
		m_bShake[i] = false;
	}
}

void CState_WDR_SizemicHammer::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_SizemicHammer::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_SizemicHammer::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSizemicHammer);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		if (iAnimFrame >= 41 && false == m_bShake[0])
		{
			Effect_Skill();
			m_pPlayer->Get_Camera()->Cam_Shake(0.05f, 100.0f, 0.5f, 10.0f);
			m_bShake[0] = true;
		}

		if (iAnimFrame >= 60 && false == m_bShake[1])
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.5f, 10.0f);
			m_bShake[1] = true;
		}
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSizemicHammer))
		m_pPlayer->Set_State(TEXT("Idle"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	if (100 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSizemicHammer))
	{
		if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Attack_1"));
		}
		else if (true == static_cast<CController_WDR*>(m_pController)->Is_Identity())
		{
			m_pPlayer->Set_State(TEXT("WDR_Identity"));
		}
		else if (true == m_pController->Is_Run())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			{
				m_pPlayer->Set_TargetPos(vClickPos);
				m_pPlayer->Set_State(TEXT("Run"));
			}
		}
	}
}

void CState_WDR_SizemicHammer::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSizemicHammer);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;

		if (iAnimFrame >= 41 && false == m_bShake[0])
		{
			Effect_Skill();
			m_bShake[0] = true;
		}
	}
}

void CState_WDR_SizemicHammer::Effect_Skill()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = matWorld.Translation();
	Vec3 vLook = matWorld.Backward();
	vLook.Normalize();
	Vec3 vRight = matWorld.Right();
	vRight.Normalize();

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 4.2f;

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vLook;
		tDesc.vScale = Vec3(1.0f, 2.0f, 1.0f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 2.5f;
		vEffectPos.y += 0.3f;

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vLook;
		tDesc.vScale = Vec3(0.7f, 1.0f, 0.5f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 3.8f + vRight * 0.5f;
		Vec3 vEffectLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(35.0f)));

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vEffectLook;
		tDesc.vScale = Vec3(0.7f, 2.0f, 1.0f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}
	
	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 3.3f + vRight * 0.8f;
		Vec3 vEffectLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(45.0f)));

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vEffectLook;
		tDesc.vScale = Vec3(0.7f, 1.5f, 0.8f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 2.5f + vRight * 1.7f;
		Vec3 vEffectLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(65.0f)));

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vEffectLook;
		tDesc.vScale = Vec3(1.0f, 1.0f, 1.0f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 3.5f + vRight * -1.2f;
		Vec3 vEffectLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-45.0f)));

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vEffectLook;
		tDesc.vScale = Vec3(1.0f, 1.0f, 1.0f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}

	{
		Vec3 vEffectPos = vPos;
		vEffectPos = vPos + vLook * 2.5f + vRight * -1.7f;
		Vec3 vEffectLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(-65.0f)));

		CEffect_Custom_SeismicHammerRock::EffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vEffectLook;
		tDesc.vScale = Vec3(1.0f, 1.5f, 0.8f);

		CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SeismicHammerRock", &tDesc);

	}


	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START(L"Seismic1", &tDesc);
	EFFECT_START(L"Seismic2", &tDesc);
	EFFECT_START(L"Seismic3", &tDesc);
}

CState_WDR_SizemicHammer* CState_WDR_SizemicHammer::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_SizemicHammer* pInstance = new CState_WDR_SizemicHammer(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_SizemicHammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_SizemicHammer::Free()
{
	__super::Free();
}
