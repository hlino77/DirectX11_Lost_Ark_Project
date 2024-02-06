#include "stdafx.h"
#include "..\Public\State_SP_Moondrawing_Start.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"

CState_SP_Moondrawing_Start::CState_SP_Moondrawing_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Moondrawing_Start::Initialize()
{
	m_iMoondrawing_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_moondrawing_01", 1.0f);
	if (m_iMoondrawing_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Moondrawing_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Moondrawing_Start::Tick_State_NoneControl;

	m_SkillFrames.push_back(13);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Moondrawing_Start::Enter_State()
{
	m_iSkillCnt = 0;
	m_bComboContinue = false;

	m_pPlayer->Reserve_Animation(m_iMoondrawing_Start, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_SP_Moondrawing_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Moondrawing_Start::Exit_State()
{
	if (true == m_pController->Is_HitState() && 45 > m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iMoondrawing_Start))
	{
		m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_SP_Moondrawing_Start::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iMoondrawing_Start);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iMoondrawing_Start))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pPlayer->Get_SP_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		15 <= iAnimFrame &&
		25 > iAnimFrame)
	{
		m_bComboContinue = true;
	}

	if (25 <= iAnimFrame && true == m_bComboContinue)
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Skill_SP_Moondrawing_End"));
	}
	else if (45 <= iAnimFrame)
	{
		m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);

		_uint iIdentity = static_cast<CController_SP*>(m_pController)->Is_SP_Identity();

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
		else if (0 != iIdentity)
		{
			Vec3 vClickPos;
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);
			else
				m_pPlayer->Set_TargetPos(Vec3());

			if (1 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_Moon"));
			else if (2 == iIdentity)
				m_pPlayer->Set_State(TEXT("Identity_Sun"));
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

void CState_SP_Moondrawing_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_Moondrawing_Start* CState_SP_Moondrawing_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Moondrawing_Start* pInstance = new CState_SP_Moondrawing_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Moondrawing_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Moondrawing_Start::Free()
{
	__super::Free();
}
