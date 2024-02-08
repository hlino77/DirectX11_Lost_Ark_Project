#include "stdafx.h"
#include "..\Public\State_WDR_EarthEater.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Custom_EarthEaterParticle.h"
#include "Pool.h"
#include "Effect_Custom_EarthEaterSmallParticle.h"
#include "Effect_Manager.h"
#include "Effect_Custom_EarthEaterDecal.h"
#include "Camera_Player.h"

CState_WDR_EarthEater::CState_WDR_EarthEater(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_EarthEater::Initialize()
{
	m_iEarthEater = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_eartheater", 1.2f);
	if (m_iEarthEater == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_EarthEater::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_EarthEater::Tick_State_NoneControl;

	m_SkillFrames.push_back(23);
	m_SkillFrames.push_back(48);
	m_SkillFrames.push_back(58);
	m_SkillFrames.push_back(68);
	m_SkillFrames.push_back(91);
	m_SkillFrames.push_back(-1);

	for (size_t i = 0; i < 2; i++)
	{
		m_bEffectOn[i] = false;
	}

	return S_OK;
}

void CState_WDR_EarthEater::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iEarthEater, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_UseMarbleMessage();

	m_bSwing = false;

	for (size_t i = 0; i < 2; i++)
	{
		m_bEffectOn[i] = false;
	}
}

void CState_WDR_EarthEater::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_EarthEater::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	m_Particles.clear();
	m_SmallParticles.clear();
}

void CState_WDR_EarthEater::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEarthEater);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_uint)iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		if (iAnimFrame >= 23 && false == m_bEffectOn[0])
		{
			Effect_Start();
			m_bEffectOn[0] = true;
		}


		if (iAnimFrame >= 91 && false == m_bEffectOn[1])
		{
			Effect_End();
			m_bEffectOn[1] = true;
		}

	}

	if (m_bSwing == false && iAnimFrame >= 88)
	{
		Effect_Swing();
		m_bSwing = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iEarthEater))
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
	if (110 <= iAnimFrame)
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

void CState_WDR_EarthEater::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iEarthEater);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_uint)iAnimFrame)
	{
		m_iSkillCnt++;

		if (iAnimFrame >= 23 && false == m_bEffectOn[0])
		{
			Effect_Start();
			m_bEffectOn[0] = true;
		}


		if (iAnimFrame >= 91 && false == m_bEffectOn[1])
		{
			Effect_End();
			m_bEffectOn[1] = true;
		}

	}

	if (m_bSwing == false && iAnimFrame >= 88)
	{
		Effect_Swing();
		m_bSwing = true;
	}
}

void CState_WDR_EarthEater::Effect_Start()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	Vec3 vPos = matWorld.Translation();
	Vec3 vLook = matWorld.Backward();
	vLook.Normalize();
	Vec3 vEffectPos = vPos - vLook * 1.0f;

	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START(L"EarthEater1", &tDesc);

	{
		CEffect_Custom_EarthEaterParticle::CustomEffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vLook;
		tDesc.vPlayerPos = vPos;

		for (_uint i = 0; i < 5; ++i)
		{
			CEffect_Custom_EarthEaterParticle* pEffect = CPool<CEffect_Custom_EarthEaterParticle>::Get_Obj();
			pEffect->Reset(tDesc);
			m_Particles.push_back(pEffect);
		}
	}
	
	{
		CEffect_Custom_EarthEaterSmallParticle::CustomEffectDesc tDesc;
		tDesc.vPos = vEffectPos;
		tDesc.vLook = vLook;
		tDesc.vPlayerPos = vPos;

		for (_uint i = 0; i < 10; ++i)
		{
			CEffect_Custom_EarthEaterSmallParticle* pEffect = CPool<CEffect_Custom_EarthEaterSmallParticle>::Get_Obj();
			pEffect->Reset(tDesc);
			m_SmallParticles.push_back(pEffect);
		}

	}

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 40.0f, 0.5f, 10.0f);
	
	Safe_Release(pGameInstance);
}

void CState_WDR_EarthEater::Effect_Swing()
{
	for (auto& Effect : m_Particles)
	{
		Effect->Set_Swing();
	}


	for (auto& Effect : m_SmallParticles)
	{
		Effect->Set_Swing();
	}
}

void CState_WDR_EarthEater::Effect_End()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = matWorld.Translation();
	Vec3 vLook = matWorld.Backward();
	vLook.Normalize();

	{
		CEffect_Custom_EarthEaterDecal::CustomEffectDesc tDesc;
		tDesc.vPos = vPos + vLook * 1.6f;
		tDesc.vLook = vLook;

		CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_EarthEaterDecal", &tDesc);

	}

	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = &matWorld;
		EFFECT_START(L"EarthEater2", &tDesc);
	}

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.5f, 10.0f);
}

void CState_WDR_EarthEater::Effect_Exit()
{
	for (auto& Effect : m_Particles)
	{
		Effect->Set_Dead(true);
	}


	for (auto& Effect : m_SmallParticles)
	{
		Effect->Set_Dead(true);
	}


}

CState_WDR_EarthEater* CState_WDR_EarthEater::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_EarthEater* pInstance = new CState_WDR_EarthEater(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_EarthEater");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_EarthEater::Free()
{
	__super::Free();
}
