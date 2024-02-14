#include "stdafx.h"
#include "..\Public\State_SP_Inkpaddle.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_SP_Inkpaddle::CState_SP_Inkpaddle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Inkpaddle::Initialize()
{
	m_iInkpaddle = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_inkpaddle", 1.0f);
	if (m_iInkpaddle == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Inkpaddle::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Inkpaddle::Tick_State_NoneControl;

	m_SkillFrames.push_back(17);
	m_SkillFrames.push_back(-1);


	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_219.wav"))); // Player
	m_SoundFrames.push_back(SOUNDDESC(100, TEXT("Effect"), TEXT("SP_116.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_SP_Inkpaddle::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);
	}
	

	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iInkpaddle, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;
	m_bTrail = false;



}

void CState_SP_Inkpaddle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Inkpaddle::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	TrailEnd();

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_SP_Inkpaddle::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iInkpaddle);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		if (m_pPlayer->Is_Control())
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 100.0f, 0.2f, 10.0f);
		}
	}

	if (m_bEffect == false && 12 <= iAnimFrame)
	{
		Effect_Shot();
		m_bEffect = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iInkpaddle))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (30 <= iAnimFrame)
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

void CState_SP_Inkpaddle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iInkpaddle);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (m_bEffect == false && 12 <= iAnimFrame)
	{
		Effect_Shot();
		m_bEffect = true;
	}
}

void CState_SP_Inkpaddle::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"InkPaddle", &tDesc);
}

void CState_SP_Inkpaddle::Effect_Trail()
{
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("InkPaddleTrail"), func, m_Trails);
}

void CState_SP_Inkpaddle::TrailEnd()
{
	for (auto& Trail : m_Trails)
	{
		dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(3);
	}
	m_Trails.clear();
}

CState_SP_Inkpaddle* CState_SP_Inkpaddle::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Inkpaddle* pInstance = new CState_SP_Inkpaddle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Inkpaddle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Inkpaddle::Free()
{
	__super::Free();
}
