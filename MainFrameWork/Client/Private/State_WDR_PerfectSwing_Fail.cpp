#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Fail.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect.h"


CState_WDR_PerfectSwing_Fail::CState_WDR_PerfectSwing_Fail(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PerfectSwing_Fail::Initialize()
{
	m_iPerfectSwing_Fail = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_attack01", 1.2f);
	if (m_iPerfectSwing_Fail == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PerfectSwing_Fail::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PerfectSwing_Fail::Tick_State_NoneControl;
	
	m_SkillFrames.push_back(24);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_PerfectSwing_Fail::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPerfectSwing_Fail, 0.1f, 0, 0, 1.f);

	m_bEffectEnd = false;
}

void CState_WDR_PerfectSwing_Fail::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Fail::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_PerfectSwing_Fail::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPerfectSwing_Fail);

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPerfectSwing_Fail))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (m_bEffectEnd == false)
	{
		if (iAnimFrame >= 29)
		{
			Effect_End();
			m_bEffectEnd = true;
		}
		else
		{
			Update_Effect();
		}

	}

	if (55 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

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

void CState_WDR_PerfectSwing_Fail::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_WDR_PerfectSwing_Fail::Update_Effect()
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

	m_pPlayer->Get_Effect(L"PerfectCircle")->Update_Pivot(matWorld);
}

void CState_WDR_PerfectSwing_Fail::Effect_End()
{
	m_pPlayer->Delete_Effect(L"PerfectCircle");
}

CState_WDR_PerfectSwing_Fail* CState_WDR_PerfectSwing_Fail::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PerfectSwing_Fail* pInstance = new CState_WDR_PerfectSwing_Fail(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PerfectSwing_Fail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PerfectSwing_Fail::Free()
{
	__super::Free();
}
