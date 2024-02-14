#include "stdafx.h"
#include "..\Public\State_WR_FuriousClaw_Loop.h"
#include "StateMachine.h"
#include "Player_Slayer.h"
#include "Controller_WR.h"
#include "Player_Skill.h"
#include "Effect_Manager.h"
#include "Model.h"

CState_WR_FuriousClaw_Loop::CState_WR_FuriousClaw_Loop(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WR_FuriousClaw_Loop::Initialize()
{
	m_iFuriousClaw_Loop = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_furiousclaw_02", 1.f);
	if (m_iFuriousClaw_Loop == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WR_FuriousClaw_Loop::Tick_State_Control;
	else
		m_TickFunc = &CState_WR_FuriousClaw_Loop::Tick_State_NoneControl;

	m_SkillFrames.push_back(3);
	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(17);
	m_SkillFrames.push_back(-1);
	

	
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Furi_126.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(1, TEXT("Effect"), TEXT("WR_Furi_127.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(8, TEXT("Effect"), TEXT("WR_Furi_126.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(8, TEXT("Effect"), TEXT("WR_Furi_127.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(15, TEXT("Effect"), TEXT("WR_Furi_126.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC(15, TEXT("Effect"), TEXT("WR_Furi_127.wav"), 0.4f));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_WR_FuriousClaw_Loop::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iFuriousClaw_Loop, 0.1f, 0, 0);
	if (true == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_Loop, 1.2f);
	else
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_Loop, 1.f);

	m_iSkillCnt = 0;
	m_iEffectCnt = 0;
	m_iSoundCnt = 0;

	m_bTrail = false;
	m_Trail.clear();

	for (_int i = 0; i < 3; ++i)
		m_EffectStart[i] = false;

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_WR_FuriousClaw_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WR_FuriousClaw_Loop::Exit_State()
{
	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_WR_FuriousClaw_Loop::Tick_State_Control(_float fTimeDelta)
{
	_int iCurrFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFuriousClaw_Loop);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= iCurrFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	if (false == m_EffectStart[m_iEffectCnt] && -1 != m_SkillFrames[m_iEffectCnt] && m_SkillFrames[m_iEffectCnt] - 1 <= iCurrFrame)
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

		if (static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		{
			if (0 == m_iEffectCnt)
			{
				EFFECT_START(TEXT("Slayer_Rage_FuriousClaw1"), &desc)

				if (!m_bTrail)
				{
					m_bTrail = true;
					
					auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
					TRAIL_START_OUTLIST(TEXT("Slayer_Rage_FuriousClaw_Trail"), func, m_Trail)
				}
			}
			else if (1 == m_iEffectCnt)
				EFFECT_START(TEXT("Slayer_Rage_FuriousClaw2"), &desc)
			else if (2 == m_iEffectCnt)
				EFFECT_START(TEXT("Slayer_Rage_FuriousClaw3"), &desc)
		}
		else
		{
			if (0 == m_iEffectCnt)
				EFFECT_START(TEXT("Slayer_FuriousClaw1"), &desc)
			else if (1 == m_iEffectCnt)
				EFFECT_START(TEXT("Slayer_FuriousClaw2"), &desc)
			else if (2 == m_iEffectCnt)
				EFFECT_START(TEXT("Slayer_FuriousClaw3"), &desc)
		}

		m_EffectStart[m_iEffectCnt] = true;
		++m_iEffectCnt;
	}

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iCurrFrame)
	{
		m_iSkillCnt++;
		m_pController->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFuriousClaw_Loop))
		m_pPlayer->Set_State(TEXT("Skill_WR_FuriousClaw_End"));

	Vec3 vClickPos;
	if (true == m_pController->Is_Dash())
	{
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);

		m_pPlayer->Set_State(TEXT("Dash"));
	}

	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_Loop, 1.f);
}

void CState_WR_FuriousClaw_Loop::Tick_State_NoneControl(_float fTimeDelta)
{
	if (false == static_cast<CController_WR*>(m_pController)->Is_In_Identity())
		m_pPlayer->Get_ModelCom()->Set_Anim_Speed(m_iFuriousClaw_Loop, 1.f);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_WR_FuriousClaw_Loop* CState_WR_FuriousClaw_Loop::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Slayer* pOwner)
{
	CState_WR_FuriousClaw_Loop* pInstance = new CState_WR_FuriousClaw_Loop(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WR_FuriousClaw_Loop");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WR_FuriousClaw_Loop::Free()
{
	__super::Free();
}
