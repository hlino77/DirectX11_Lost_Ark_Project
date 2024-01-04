#include "stdafx.h"
#include "..\Public\State_WDR_PowerShoulder_Start.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"

CState_WDR_PowerShoulder_Start::CState_WDR_PowerShoulder_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PowerShoulder_Start::Initialize()
{
	m_iPowerShoulder_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_powershoulder_01", 1.f);
	if (m_iPowerShoulder_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PowerShoulder_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PowerShoulder_Start::Tick_State_NoneControl;

	m_SkillFrames.push_back(14);
	m_SkillFrames.push_back(18);
	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(26);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_WDR_PowerShoulder_Start::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPowerShoulder_Start, 0.1f, 0, 0);

	m_pPlayer->Get_WDR_Controller()->Get_StopMessage();
	m_pPlayer->Get_WDR_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_pPlayer->Get_WDR_Controller()->Get_AddMarbleMessage(2);
}

void CState_WDR_PowerShoulder_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PowerShoulder_Start::Exit_State()
{
	if (29 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start))
	{
		m_pPlayer->Get_WDR_Controller()->Get_SkillMessage(m_eSkillSelectKey);

		if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
			m_pPlayer->Set_SuperArmorState(false);
	}
	m_bComboContinue = false;
}

void CState_WDR_PowerShoulder_Start::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start))
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (-1.f == m_pPlayer->Get_TargetPos().y)
	{
		m_pPlayer->Get_ModelCom()->Save_PreFrame(m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start));
		m_pPlayer->Set_State(TEXT("Skill_WDR_PowerShoulder_Loop"));
	}
	if (true == m_pPlayer->Get_WDR_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		10 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start) &&
		29 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start))
	{
		m_bComboContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPowerShoulder_Start))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (29 == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start))
	{
		if (true == m_bComboContinue)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PowerShoulder_End"));
		}
	}
	else if (29 < m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start))
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

void CState_WDR_PowerShoulder_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WDR_PowerShoulder_Start* CState_WDR_PowerShoulder_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PowerShoulder_Start* pInstance = new CState_WDR_PowerShoulder_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PowerShoulder_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PowerShoulder_Start::Free()
{
	__super::Free();
}
