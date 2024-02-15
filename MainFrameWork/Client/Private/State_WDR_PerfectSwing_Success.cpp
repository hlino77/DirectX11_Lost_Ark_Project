#include "stdafx.h"
#include "..\Public\State_WDR_PerfectSwing_Success.h"
#include "StateMachine.h"
#include "Player_Destroyer.h"
#include "Controller_WDR.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_WDR_PerfectSwing_Success::CState_WDR_PerfectSwing_Success(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_WDR_PerfectSwing_Success::Initialize()
{
	m_iPerfect_Success = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectswing_attack02", 1.2f);
	if (m_iPerfect_Success == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_WDR_PerfectSwing_Success::Tick_State_Control;
	else
		m_TickFunc = &CState_WDR_PerfectSwing_Success::Tick_State_NoneControl;

	m_SkillFrames.push_back(33);
	m_SkillFrames.push_back(-1);



	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_186.wav"))); //  Player  
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_86.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("WDR_85.wav"))); //  Skill
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_WDR_PerfectSwing_Success::Enter_State()
{

	m_EffectSound = false;

	if (m_pPlayer->Is_Control())
	{
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt+2].strName, m_SoundFrames[m_iSoundCnt+2].strGroup, m_SoundFrames[m_iSoundCnt+2].strName, m_SoundFrames[m_iSoundCnt+2].fVolume);

	}


	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPerfect_Success, 0.1f, 0, 0, 1.f);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffectEnd = false;
	m_bEffectTrail = false;

	if (m_pPlayer->Is_Control())
		Init_Camera();
}

void CState_WDR_PerfectSwing_Success::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_WDR_PerfectSwing_Success::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (m_pPlayer->Is_Control())
	{
		Reset_Camera();
	}

	if (true == m_pPlayer->Is_CancelState())
	{
		Effect_End();
	}


}

void CState_WDR_PerfectSwing_Success::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_iPerfect_Success);

	if (false == CSound_Manager::GetInstance()->Is_Channel_Playing(m_SoundFrames[m_iSoundCnt].strName))
	{

		if (false == m_EffectSound)
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);

			m_EffectSound = true;
		}

	}




	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_WDR*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);

		Effect_Shot();
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPerfect_Success))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (m_bEffectTrail == false && iAnimFrame >= 25)
	{
		Effect_Trail();
		m_bEffectTrail = true;
	}

	if (m_bEffectEnd == false)
	{
		if (iAnimFrame >= 37)
		{
			Effect_End();
			m_bEffectEnd = true;
		}
		else
		{
			Update_Effect();
		}
		
	}

	if(m_pPlayer->Get_Camera()->Get_Mode() == CCamera_Player::CameraState::FREE)
		Update_Camera(fTimeDelta);

	if (65 <= iAnimFrame)
	{
		Vec3 vClickPos;
		if (true == m_pController->Is_Dash())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(TEXT("Dash"));
		}
		else if (true == m_pController->Is_Skill())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

			m_pPlayer->Set_State(m_pController->Get_SkillStartName(m_pController->Get_Selected_Skill()));
		}
		else if (true == m_pController->Is_Attack())
		{
			if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
				m_pPlayer->Set_TargetPos(vClickPos);

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

void CState_WDR_PerfectSwing_Success::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPerfect_Success);

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iPerfect_Success))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (m_bEffectTrail == false && iAnimFrame >= 25)
	{
		Effect_Trail();
		m_bEffectTrail = true;
	}

	if (m_bEffectEnd == false)
	{
		if (iAnimFrame >= 37)
		{
			Effect_End();
			m_bEffectEnd = true;
		}
		else
		{
			Update_Effect();
		}

	}
}

void CState_WDR_PerfectSwing_Success::Effect_Trail()
{
	vector<CEffect*> Effects;
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("PerfectSwingTrail"), func, m_Trails);
}

void CState_WDR_PerfectSwing_Success::Update_Effect()
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

	m_pPlayer->Get_Effect(L"PerfectCircle")->Update_Pivot(matWorld);
}

void CState_WDR_PerfectSwing_Success::Effect_End()
{
	m_pPlayer->Delete_Effect(L"PerfectCircle");
	
	for (auto& Trail : m_Trails)
	{
		dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(1.0f);
	}
	m_Trails.clear();
}

void CState_WDR_PerfectSwing_Success::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

void CState_WDR_PerfectSwing_Success::Effect_Shot()
{
	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	vPos += vLook * 0.5f;
	vPos.y += 0.7f;

	m_pPlayer->Get_Camera()->Set_RadialBlur(0.5f, vPos, 0.1f, 0.1f);
	m_pPlayer->Get_Camera()->Cam_Shake(0.2f, 100.0f, 0.5f, 10.0f);
	m_pPlayer->Get_Camera()->Set_Chromatic(0.5f, vPos, 0.1f, 0.1f);
}

void CState_WDR_PerfectSwing_Success::Init_Camera()
{
	Vec3 vTargetPos = m_pPlayer->Get_Camera()->Get_TargetPos();
	vTargetPos.y += 1.0f;
	m_vCameraTargetPos = vTargetPos;
}

void CState_WDR_PerfectSwing_Success::Update_Camera(_float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vLook = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);

	{
		Vec3 vTargetOffset = vLook * -1.0f + vUp * 0.2f;
		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = Vec3::Lerp(vOffset, vTargetOffset, 2.0f * fTimeDelta);
		pCamera->Set_Offset(vOffset);
	}
	
	{
		Vec3 vTargetPos = m_pPlayer->Get_Camera()->Get_TargetPos();
		vTargetPos = Vec3::Lerp(vTargetPos, m_vCameraTargetPos, 0.5f * fTimeDelta);
		pCamera->Set_TargetPos(vTargetPos);
	}

	pCamera->ZoomInOut(2.5f, 2.0f);
}

CState_WDR_PerfectSwing_Success* CState_WDR_PerfectSwing_Success::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Destroyer* pOwner)
{
	CState_WDR_PerfectSwing_Success* pInstance = new CState_WDR_PerfectSwing_Success(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_WDR_PerfectSwing_Success");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_WDR_PerfectSwing_Success::Free()
{
	__super::Free();
}
