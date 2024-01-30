#include "stdafx.h"
#include "..\Public\State_MG_SonicVibe_Start.h"
#include "StateMachine.h"
#include "Player_Bard.h"
#include "Controller_MG.h"
#include "Player_Skill.h"
#include "Model.h"

CState_MG_SonicVibe_Start::CState_MG_SonicVibe_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_MG_SonicVibe_Start::Initialize()
{
	m_iSonicVibe = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_battle_1", 1.2f);
	if (m_iSonicVibe == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_MG_SonicVibe_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_MG_SonicVibe_Start::Tick_State_NoneControl;

	return S_OK;
}

void CState_MG_SonicVibe_Start::Enter_State()
{
	if(TEXT("Idle") != m_pPlayer->Get_PreState())
		m_pPlayer->Reserve_Animation(m_iSonicVibe, 0.1f, 0, 0);

	m_pPlayer->Get_MG_Controller()->Get_StopMessage();

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_fTimeAcc = 0.0f;
}

void CState_MG_SonicVibe_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_MG_SonicVibe_Start::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_MG_SonicVibe_Start::Tick_State_Control(_float fTimeDelta)
{
	Vec3 vClickPos;
	if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
	}
	else
	{
		m_pPlayer->Set_TargetPos(Vec3());
	}
		

	if (true == m_pController->Is_Tap(KEY::LBTN))
	{
		Vec3 vDir = m_pPlayer->Get_TargetPos() - m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

		if (4.f <= vDir.Length())
		{
			m_vColliPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
			vDir.Normalize();
			m_vColliPos += vDir * 4.f;
		}
		else
		{
			m_vColliPos = m_pPlayer->Get_TargetPos();
		}

		m_pPlayer->Set_TargetPos(m_vColliPos);
		m_pPlayer->Set_State(TEXT("Skill_MG_SonicVibe_End"));
	}
	
	m_fTimeAcc += fTimeDelta;
	if (m_fTimeAcc >= m_fReleaseTime)
	{
		if (true == m_pController->Is_Dash())
		{
			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Run())
		{
			m_pPlayer->Set_State(TEXT("Run"));
		}
	}	
	if (m_fTimeAcc >= m_fEndTime)
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}
}

void CState_MG_SonicVibe_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_MG_SonicVibe_Start* CState_MG_SonicVibe_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Bard* pOwner)
{
	CState_MG_SonicVibe_Start* pInstance = new CState_MG_SonicVibe_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_MG_SonicVibe_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_MG_SonicVibe_Start::Free()
{
	__super::Free();
}
