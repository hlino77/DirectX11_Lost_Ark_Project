#include "stdafx.h"
#include "..\Public\State_GN_DeadHard_Loop.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "UI_HoldingFrame.h"

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

	m_SkillFrames.push_back(0);
	m_SkillFrames.push_back(3);
	m_SkillFrames.push_back(6);
	m_SkillFrames.push_back(9);
	m_SkillFrames.push_back(12);
	m_SkillFrames.push_back(15);
	m_SkillFrames.push_back(18);
	m_SkillFrames.push_back(21);
	m_SkillFrames.push_back(-1);


	m_EffectFrames.push_back(EFFECTFRAMEDESC(0, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(3, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(6, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(9, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(12, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(15, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(18, (_uint)CPartObject::PARTS::WEAPON_1));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(21, (_uint)CPartObject::PARTS::WEAPON_2));
	m_EffectFrames.push_back(EFFECTFRAMEDESC());

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("GN_Deadhard_70.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	if (m_pPlayer->Is_Control())
	{
		CUI_HoldingFrame::HOLDING_SKILL_DESC HoldingDesc;
		HoldingDesc.strSkillName = TEXT("레인 오브 불릿");
		HoldingDesc.fSkillTimeAcc = m_fSkillTimeAcc;
		HoldingDesc.fSkillEndTime = m_fSkillEndTime;
		HoldingDesc.fSkillSuccessTime_Min = m_fSkillSuccessTime_Min;
		HoldingDesc.fSkillSuccessTime_Max = m_fSkillSuccessTime_Max;
		m_pHoldingUI = static_cast<CUI_HoldingFrame*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC,
			_uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Skill_HoldingGauge"), &HoldingDesc));
		if (nullptr == m_pHoldingUI)
			return E_FAIL;
	}
	return S_OK;
}

void CState_GN_DeadHard_Loop::Enter_State()
{
	m_pController->Get_MoveSpeedMessage(2.5f);

	m_iSkillCnt = 0;
	m_iEffectCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
	m_pPlayer->Set_TargetPos(Vec3());
	m_iDeadHard = m_iDeadHard_Loop;

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(true);
}

void CState_GN_DeadHard_Loop::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_DeadHard_Loop::Exit_State()
{
	m_fSkillTimeAcc = 0.f;
	m_iDeadHard = m_iDeadHard_Loop;

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillOn(false);

	StopStateSound();
}

void CState_GN_DeadHard_Loop::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeadHard);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame == iAnimFrame)
	{
		Effect_Shot();

		m_iEffectCnt++;
	}

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
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

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDeadHard))
	{
		m_iSkillCnt = 0;
		m_iEffectCnt = 0;
	}

	Vec3 vClickPos;
	if (true == m_pController->Is_HoldorTap(KEY::RBTN) && true == m_pPlayer->Get_CellPickingPos(vClickPos))
	{
		m_pPlayer->Set_TargetPos(vClickPos);
		m_pController->Get_DirMessage(vClickPos);

		_float fDegree = m_pPlayer->Get_TransformCom()->Get_TargetDegree(vClickPos);

		if (m_iDeadHard != m_iDeadHard_F && (-80.f <= fDegree && 80.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_F, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_F;
			m_iSkillCnt = 0;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_R && (80.f < fDegree && 100.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_R, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_R;
			m_iSkillCnt = 0;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_L && (-80.f > fDegree && -100.f < fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_L, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_L;
			m_iSkillCnt = 0;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_B && (-100.f >= fDegree || 100.f <= fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_B, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_B;
			m_iSkillCnt = 0;
			m_iEffectCnt = 0;
		}
	}
	else if (m_iDeadHard != m_iDeadHard_Loop && true == m_pController->Is_Stop())
	{
		m_pController->Get_StopMessage();
		m_pPlayer->Set_TargetPos(Vec3());
		m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
		m_iDeadHard = m_iDeadHard_Loop;
		m_iSkillCnt = 0;
		m_iEffectCnt = 0;
	}

	m_fSkillTimeAcc += fTimeDelta;
	if (nullptr != m_pHoldingUI)
		m_pHoldingUI->Set_SkillTimeAcc(m_fSkillTimeAcc);
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
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeadHard);

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
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


	Vec3 vTargetPos = m_pPlayer->Get_TargetPos();

	if (vTargetPos == Vec3() && true == m_pController->Is_Stop())
	{
		m_pController->Get_StopMessage();
		if (m_iDeadHard != m_iDeadHard_Loop)
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_Loop, 0.1f, 0, 0);
			m_iDeadHard = m_iDeadHard_Loop;
			m_iEffectCnt = 0;
		}
	}
	else
	{
		m_pController->Get_DirMessage(vTargetPos);

		_float fDegree = m_pPlayer->Get_TransformCom()->Get_TargetDegree(vTargetPos);

		if (m_iDeadHard != m_iDeadHard_F && (-80.f <= fDegree && 80.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_F, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_F;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_R && (80.f < fDegree && 100.f > fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_R, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_R;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_L && (-80.f > fDegree && -100.f < fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_L, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_L;
			m_iEffectCnt = 0;
		}
		else if (m_iDeadHard != m_iDeadHard_B && (-100.f >= fDegree || 100.f <= fDegree))
		{
			m_pPlayer->Reserve_Animation(m_iDeadHard_B, 0.3f, 0, 0);
			m_iDeadHard = m_iDeadHard_B;
			m_iEffectCnt = 0;
		}
	}


	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_iDeadHard);

	if (-1 != m_EffectFrames[m_iEffectCnt].iFrame && m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		Effect_Shot();

		m_iEffectCnt++;
	}
}

void CState_GN_DeadHard_Loop::Effect_Shot()
{
	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts((CPartObject::PARTS)m_EffectFrames[m_iEffectCnt].iWeapon))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("HandBullet"), &desc)


	_uint iCount = rand() % 2 + 1;
	for (_uint i = 0; i < iCount; ++i)
	{
		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(TEXT("HandBulletParticle"), &desc)
	}
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
	m_pHoldingUI->Set_Dead(true);
}
