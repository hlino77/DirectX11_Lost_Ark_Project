#include "stdafx.h"
#include "..\Public\State_SP_Inkshot.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_SP_Inkshot::CState_SP_Inkshot(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Inkshot::Initialize()
{
	m_iInkshot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_inkshot", 1.0f);
	if (m_iInkshot == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Inkshot::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Inkshot::Tick_State_NoneControl;

	m_SkillFrames.push_back(20);
	m_SkillFrames.push_back(-1);

	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_118.wav"))); // Skill   0
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_119.wav"))); // Skill   1
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_215.wav"))); // Player  2
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_212.wav"))); // Player  3
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_SP_Inkshot::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	if (m_pPlayer->Is_Control())
	{
		//// Skill Sound 
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);

		//// Player Sound
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].strGroup, m_SoundFrames[m_iSoundCnt + 2].strName, m_SoundFrames[m_iSoundCnt + 2].fVolume);

	}

	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iInkshot, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;
	m_bChargeEffect = false;



}

void CState_SP_Inkshot::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);

}

void CState_SP_Inkshot::Exit_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	Charge_End();
}

void CState_SP_Inkshot::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iInkshot);


	if ( true == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt + 0].strName))
	{
		if (false == m_EffectSound)
		{
			m_EffectSoundAccTime += fTimeDelta;

			if (m_EffectSoundAccTime >= 0.5f)
			{
				m_EffectSoundAccTime = 0.f;
				CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);

				m_EffectSound = true;
			}
		}
	}

	if (false == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt + 2].strName))
	{
		if (false == m_PlayerSound)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt + 3].strGroup, m_SoundFrames[m_iSoundCnt + 3].strName, m_SoundFrames[m_iSoundCnt + 3].fVolume);
			m_PlayerSound = true;
		}
	}


	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (iAnimFrame < 18)
	{
		if (m_bChargeEffect == false)
		{
			Effect_Charge();
			m_bChargeEffect = true;
		}

		Update_Effect();
	}
	else if (m_bEffect == false && 18 <= iAnimFrame)
	{
		Charge_End();
		Effect_Shot();
		m_bEffect = true;
	}


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iInkshot))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (38 <= iAnimFrame)
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

void CState_SP_Inkshot::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iInkshot);

	if (iAnimFrame < 18)
	{
		if (m_bChargeEffect == false)
		{
			Effect_Charge();
			m_bChargeEffect = true;
		}

		Update_Effect();
	}
	else if (m_bEffect == false && 18 <= iAnimFrame)
	{
		Charge_End();
		Effect_Shot();
		m_bEffect = true;
	}
}

void CState_SP_Inkshot::Effect_Charge()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = const_cast<Matrix*>(&dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix());
	EFFECT_START_OUTLIST(L"InkShotParticle1", &tDesc, m_Effects);
}

void CState_SP_Inkshot::Charge_End()
{
	for (auto& Effect : m_Effects)
	{
		Effect->EffectEnd();
	}
	m_Effects.clear();
}

void CState_SP_Inkshot::Update_Effect()
{
	Matrix matWorld = dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

	for (auto& Effect : m_Effects)
	{
		Effect->Update_Pivot(matWorld);
	}
}

void CState_SP_Inkshot::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	EFFECT_START(L"InkShotEndParticle1", &tDesc);

	if (m_pPlayer->Is_Control())
	{
		m_pPlayer->Get_Camera()->Cam_Shake(0.15f, 100.0f, 0.2f, 10.0f);
	}
}

CState_SP_Inkshot* CState_SP_Inkshot::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Inkshot* pInstance = new CState_SP_Inkshot(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Inkshot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Inkshot::Free()
{
	__super::Free();
}
