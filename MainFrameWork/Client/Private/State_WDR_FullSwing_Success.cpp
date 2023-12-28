#include "stdafx.h"
#include "..\Public\State_WDR_FullSwing_Success.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_FullSwing_Success::CState_WDR_FullSwing_Success(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_FullSwing_Success::Initialize()
{
	m_iFullSwing_Success = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_fullswing_success", 1.2f);
	if (m_iFullSwing_Success == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_FullSwing_Success::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_FullSwing_Success::Tick_State_NoneControl;

	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(24);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_FullSwing_Success::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFullSwing_Success, 0.1f, 0, 0);

	m_pController->Set_SkillSuccess(m_eSkillSelectKey, true);
}

void CState_WDR_FullSwing_Success::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_FullSwing_Success::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	m_pController->Set_SkillSuccess(m_eSkillSelectKey, false);
}

void CState_WDR_FullSwing_Success::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFullSwing_Success))
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFullSwing_Success))
		m_pPlayer->Set_State(TEXT("Idle"));


	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	if (90 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFullSwing_Success))
	{
		if (true == m_pController->Is_Skill())
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

void CState_WDR_FullSwing_Success::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_FullSwing_Success* CState_WDR_FullSwing_Success::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_FullSwing_Success* pInstance = new CState_WDR_FullSwing_Success(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_FullSwing_Success");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_FullSwing_Success::Free()
{
	__super::Free();
}
