#include "stdafx.h"
#include "..\Public\State_WDR_PowerShoulder_End.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"

CState_WDR_PowerShoulder_End::CState_WDR_PowerShoulder_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PowerShoulder_End::Initialize()
{
	m_iPowerShoulder_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_powershoulder_02", 1.f);
	if (m_iPowerShoulder_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PowerShoulder_End::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PowerShoulder_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(-1);

	m_fCircle1Time = 0.3f;
	m_fCircle2Time = 0.1f;

	return S_OK;
}

void CState_WDR_PowerShoulder_End::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPowerShoulder_End, 0.2f, 0, 0, 1.f);

	m_fCircle1TimeAcc = m_fCircle1Time;
	m_fCircle2TimeAcc = m_fCircle2Time;
}	

void CState_WDR_PowerShoulder_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PowerShoulder_End::Exit_State()
{
	m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

}

void CState_WDR_PowerShoulder_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_End);

	if (iAnimFrame < 6)
	{
		Update_Effect_Circle(fTimeDelta);
	}

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		Effect_End();
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPowerShoulder_End))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (20 <= iAnimFrame)
	{
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
}

void CState_WDR_PowerShoulder_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WDR_PowerShoulder_End::Effect_End()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(L"PowerShoulderEnd", &desc);
}

void CState_WDR_PowerShoulder_End::Update_Effect_Circle(_float fTimeDelta)
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;

	m_fCircle1TimeAcc += fTimeDelta;

	if (m_fCircle1TimeAcc >= m_fCircle1Time)
	{
		m_fCircle1TimeAcc = 0.0f;
		EFFECT_START(L"PowerShoulderDecal1", &desc);
	}

	m_fCircle2TimeAcc += fTimeDelta;

	if (m_fCircle2TimeAcc >= m_fCircle2Time)
	{
		m_fCircle2TimeAcc = 0.0f;
		EFFECT_START(L"PowerShoulderDecal2", &desc);
	}
	
}


CState_WDR_PowerShoulder_End* CState_WDR_PowerShoulder_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PowerShoulder_End* pInstance = new CState_WDR_PowerShoulder_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PowerShoulder_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PowerShoulder_End::Free()
{
	__super::Free();
}
