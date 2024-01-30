#include "stdafx.h"
#include "..\Public\State_WDR_PowerShoulder_Start.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

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

	m_fCamShakeAcc = 0.0f;
	m_fCamShakeDelay = 0.3f;

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

	m_bEffect = false;
	m_bEffectEnd = false;
	m_Effects.clear();

	m_fCamShakeAcc = m_fCamShakeDelay;
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
	}
	m_bComboContinue = false;

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WDR_PowerShoulder_Start::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start);

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (iAnimFrame > 13)
	{
		if (m_fCamShakeAcc >= m_fCamShakeDelay)
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.02f, 80.0f, 1.0f, 5.0f);
			m_fCamShakeAcc = 0.0f;
		}
		else
			m_fCamShakeAcc += fTimeDelta;
	}

	if (m_bEffect == false && iAnimFrame >= 14)
	{
		m_bEffect = true;
		Effect_Start();
	}

	if(m_bEffectEnd == false)
		Update_Effect();

	if (-1.f == m_pPlayer->Get_TargetPos().y)
	{
		m_pPlayer->Get_ModelCom()->Save_PreFrame(iAnimFrame);
		m_pPlayer->Set_State(TEXT("Skill_WDR_PowerShoulder_Loop"));
	}
	if (true == m_pPlayer->Get_WDR_Controller()->Is_HoldorTap(m_eSkillBindKey) &&
		10 <= iAnimFrame &&
		29 > iAnimFrame)
	{
		m_bComboContinue = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPowerShoulder_Start))
	{
		m_pPlayer->Set_State(TEXT("Idle"));
	}

	if (29 == iAnimFrame)
	{
		if (true == m_bComboContinue)
		{
			m_pPlayer->Set_State(TEXT("Skill_WDR_PowerShoulder_End"));
			m_bEffectEnd = true;
		}
	
	}
	else if (29 < iAnimFrame)
	{
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
}

void CState_WDR_PowerShoulder_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPowerShoulder_Start);

	if (m_bEffect == false && iAnimFrame >= 14)
	{
		m_bEffect = true;
		Effect_Start();
	}

	if (m_bEffectEnd == false)
		Update_Effect();

	if (29 < iAnimFrame)
	{
		if (m_bEffectEnd == false)
		{
			Effect_End();
			m_bEffectEnd = true;
		}
	}
}

void CState_WDR_PowerShoulder_Start::Effect_Start()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START_OUTLIST(L"WDPowerShoulder", &desc, m_Effects);

	m_pPlayer->Add_Effect(L"WDPowerShoulder1", m_Effects[0]);
	m_pPlayer->Add_Effect(L"WDPowerShoulder2", m_Effects[1]);

	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Set_MotionBlur(5.0f, 0.03f);
}

void CState_WDR_PowerShoulder_Start::Effect_End()
{
	m_pPlayer->Delete_Effect(L"WDPowerShoulder1");
	m_pPlayer->Delete_Effect(L"WDPowerShoulder2");

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.0f, 0.0f, 0.0f, 0.0f);
		m_pPlayer->Get_Camera()->Set_MotionBlur(0.0f);
	}
}

void CState_WDR_PowerShoulder_Start::Update_Effect()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	for (auto& Effect : m_Effects)
	{
		Effect->Update_Pivot(matWorld);
	}
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
