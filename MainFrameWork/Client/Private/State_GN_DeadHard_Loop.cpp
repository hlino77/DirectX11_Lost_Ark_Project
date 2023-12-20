#include "stdafx.h"
#include "..\Public\State_GN_DeadHard_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"

CState_GN_DeadHard_Loop::CState_GN_DeadHard_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_DeadHard_Loop::Initialize()
{
	m_iDeadHard_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deadhard_02", 1.0f);
	if (m_iDeadHard_Loop == -1)
		return E_FAIL;

	m_iDeadHard_F = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deadhard_fw", 1.0f);
	m_iDeadHard_B = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deadhard_bw", 1.0f);
	m_iDeadHard_L = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deadhard_lw", 1.0f);
	m_iDeadHard_R = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deadhard_rw", 1.0f);


	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_DeadHard_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_DeadHard_Loop::Tick_State_NoneControl;


	m_fSkillEndTime = 2.f;
	m_fSkillTimeAcc = 0.f;

	return S_OK;
}

void CState_GN_DeadHard_Loop::Enter_State()
{
	
	m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_TargetPos(Vec3());
	m_iDeadHard = m_iDeadHard_Loop;
}

void CState_GN_DeadHard_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_DeadHard_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;
	m_iDeadHard = m_iDeadHard_Loop;
}

void CState_GN_DeadHard_Loop::Tick_State_Control(_float fTimeDelta)
{
	Vec3 vClickPos;
	if (true == m_pController->Is_HoldorTap(KEY::RBTN) && true == m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pController->Get_DirMessage(vClickPos, 2.5f);

		_float fDegree = m_pPlayer->Get_TransformCom()->Get_TargetDegree(vClickPos);

		if (m_iDeadHard != m_iDeadHard_F && (-80.f <= fDegree && 80.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_F, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_F;
		}
		else if (m_iDeadHard != m_iDeadHard_R && (80.f < fDegree && 100.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_R, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_R;
		}
		else if (m_iDeadHard != m_iDeadHard_L && (-80.f > fDegree && -100.f < fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_L, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_L;
		}
		else if (m_iDeadHard != m_iDeadHard_B && (-100.f >= fDegree || 100.f <= fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_B, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_B;
		}
	}
	else if (m_iDeadHard != m_iDeadHard_Loop)
	{
		m_pController->Get_StopMessage();
		m_pPlayer->Set_TargetPos(Vec3());
		m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
		m_iDeadHard = m_iDeadHard_Loop;
	}

	m_fSkillTimeAcc += fTimeDelta;

	if (m_fSkillTimeAcc >= m_fSkillEndTime)
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_DeadHard_End"));
	}
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_DeadHard_End"));
	}

}

void CState_GN_DeadHard_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	Vec3 vTargetPos = m_pPlayer->Get_TargetPos();

	if (vTargetPos == Vec3())
	{
		m_pController->Get_StopMessage();
		if (m_iDeadHard != m_iDeadHard_Loop)
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
			m_iDeadHard = m_iDeadHard_Loop;
		}
	}
	else
	{
		m_pController->Get_DirMessage(vTargetPos, 2.5f);

		_float fDegree = m_pPlayer->Get_TransformCom()->Get_TargetDegree(vTargetPos);

		if (m_iDeadHard != m_iDeadHard_F && (-80.f <= fDegree && 80.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_F, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_F;
		}
		else if (m_iDeadHard != m_iDeadHard_R && (80.f < fDegree && 100.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_R, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_R;
		}
		else if (m_iDeadHard != m_iDeadHard_L && (-80.f > fDegree && -100.f < fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_L, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_L;
		}
		else if (m_iDeadHard != m_iDeadHard_B && (-100.f >= fDegree || 100.f <= fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_B, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_B;
		}
	}


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_DeadHard_Loop* CState_GN_DeadHard_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_DeadHard_Loop* pInstance = new CState_GN_DeadHard_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_DeadHard_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_DeadHard_Loop::Free()
{
	__super::Free();
}
