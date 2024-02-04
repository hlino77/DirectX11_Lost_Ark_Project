#include "stdafx.h"
#include "..\Public\State_WDR_PowerShoulder_Loop.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_WDR_PowerShoulder_Loop::CState_WDR_PowerShoulder_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PowerShoulder_Loop::Initialize()
{
	m_iPowerShoulder_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_powershoulder_01", 1.f);
	if (m_iPowerShoulder_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PowerShoulder_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PowerShoulder_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(14);
	m_SkillFrames.push_back(18);
	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(26);
	m_SkillFrames.push_back(-1);




	return S_OK;
}

void CState_WDR_PowerShoulder_Loop::Enter_State()
{
	_uint iPreFrame = m_pPlayer->Get_ModelCom()->Get_PreFrame();

	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPowerShoulder_Loop, 0.05f, iPreFrame, 0, 0.1f);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffectEnd = false;
}

void CState_WDR_PowerShoulder_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PowerShoulder_Loop::Exit_State()
{
	if (true == m_pController->Is_HitState() && 29 >= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Loop))
	{
		m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	}
	m_bComboContinue = false;

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_PowerShoulder_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Loop);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_WDR_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		10 <= iAnimFrame &&
		29 > iAnimFrame)
	{
		m_bComboContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPowerShoulder_Loop))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (29 <= iAnimFrame)
	{
		if (true == m_bComboContinue)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PowerShoulder_End"));
			m_bEffectEnd = true;
		}
	}
	else if (29 < iAnimFrame)
	{
		m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

		if (m_bEffectEnd == false)
		{
			Effect_End();
			m_bEffectEnd = true;
		}

		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
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

	if(m_bEffectEnd == false)
		Update_Effect();
}

void CState_WDR_PowerShoulder_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Loop);

	if (29 < iAnimFrame)
	{
		if (m_bEffectEnd == false)
		{
			Effect_End();
			m_bEffectEnd = true;
		}
	}

	if (m_bEffectEnd == false)
		Update_Effect();
}

void CState_WDR_PowerShoulder_Loop::Update_Effect()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	m_pPlayer->Get_Effect(L"WDPowerShoulder1")->Update_Pivot(matWorld);
	m_pPlayer->Get_Effect(L"WDPowerShoulder2")->Update_Pivot(matWorld);
}

void CState_WDR_PowerShoulder_Loop::Effect_End()
{
	m_pPlayer->Delete_Effect(L"WDPowerShoulder1");
	m_pPlayer->Delete_Effect(L"WDPowerShoulder2");

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.0f, 0.0f, 0.0f, 0.0f);
}

CState_WDR_PowerShoulder_Loop* CState_WDR_PowerShoulder_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PowerShoulder_Loop* pInstance = new CState_WDR_PowerShoulder_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PowerShoulder_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PowerShoulder_Loop::Free()
{
	__super::Free();
}
