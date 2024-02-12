#include "stdafx.h"
#include "..\Public\State_SP_Flyheaven.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_SP_Flyheaven::CState_SP_Flyheaven(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Flyheaven::Initialize()
{
	m_iFlyheaven = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_flyinheaven", 1.0f);
	if (m_iFlyheaven == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Flyheaven::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Flyheaven::Tick_State_NoneControl;

	m_SkillFrames.push_back(30);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Flyheaven::Enter_State()
{





	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFlyheaven, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bTrail = false;
}

void CState_SP_Flyheaven::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Flyheaven::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	TrailEnd();
}

void CState_SP_Flyheaven::Tick_State_Control(_float fTimeDelta)
{
	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFlyheaven);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
		static_cast<CController_SP*>(m_pController)->Get_SkillStartMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFlyheaven))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (45 <= iAnimFrame)
	{
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

void CState_SP_Flyheaven::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_SkillFrames[m_iSkillCnt] <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFlyheaven))
	{
		Effect_Shot();
		static_cast<CController_SP*>(m_pController)->Get_SkillStartMessage(m_eSkillSelectKey);

		m_iSkillCnt++;
	}

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}
}

void CState_SP_Flyheaven::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"FlyHeaven", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.2f, 10.0f);
	}
}

void CState_SP_Flyheaven::Effect_Trail()
{
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("FlyHeavenTrail"), func, m_Trails);
}

void CState_SP_Flyheaven::TrailEnd()
{
	for (auto& Effect : m_Trails)
	{
		dynamic_cast<CEffect_Trail*>(Effect)->TrailEnd(3.0f);
	}
	m_Trails.clear();
}

CState_SP_Flyheaven* CState_SP_Flyheaven::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Flyheaven* pInstance = new CState_SP_Flyheaven(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Flyheaven");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Flyheaven::Free()
{
	__super::Free();
}
