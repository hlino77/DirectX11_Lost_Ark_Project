#include "stdafx.h"
#include "..\Public\State_SP_Onestroke.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"
#include "ServerSessionManager.h"

CState_SP_Onestroke::CState_SP_Onestroke(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Onestroke::Initialize()
{
	m_iOnestroke = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_onestroke", 1.0f);
	if (m_iOnestroke == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Onestroke::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Onestroke::Tick_State_NoneControl;

	m_SkillFrames.push_back(44);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_SP_Onestroke::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iOnestroke, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bBrush = false;
	m_bEffect = false;
	m_bBrushUpdate = true;

	m_Effects.clear();

	if (m_pPlayer->Is_Control())
		Init_Camera();
}

void CState_SP_Onestroke::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Onestroke::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (m_Effects.empty() == false)
	{
		for (auto& Effect : m_Effects)
		{
			Effect->EffectEnd();
		}
	}

	if (m_StartParticle.empty() == false)
	{
		for (auto& Particle : m_StartParticle)
		{
			Particle->EffectEnd();
		}
	}

	if(m_pPlayer->Is_Control())
		Reset_Camera();
}

void CState_SP_Onestroke::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iOnestroke);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CController_SP*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (iAnimFrame < 40)
	{
		Update_StartParticle();
	}


	if (m_bBrush == false && iAnimFrame >= 40)
	{
		Effect_Brush();
		End_StartParticle();
		m_bBrush = true;
	}

	if (m_bBrush == true && m_bBrushUpdate == true)
	{
		Update_Brush();

		if (iAnimFrame >= 43)
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.5f, 100.0f, 1.0f, 10.0f);
			m_pPlayer->Get_Camera()->Set_RadialBlur(0.5f, m_vRadialPos, 0.1f, 0.1f);
			m_bBrushUpdate = false;
		}
	}

	if (m_bEffect == false && iAnimFrame >= 41)
	{
		Effect_Shot();
		m_bEffect = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iOnestroke))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (iAnimFrame < 80)
	{
		Update_Camera(iAnimFrame, fTimeDelta);
	}


	if (80 <= iAnimFrame)
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

void CState_SP_Onestroke::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iOnestroke);

	if (iAnimFrame < 40)
	{
		Update_StartParticle();
	}


	if (m_bBrush == false && iAnimFrame >= 40)
	{
		Effect_Brush();
		End_StartParticle();
		m_bBrush = true;
	}

	if (m_bBrush == true && m_bBrushUpdate == true)
	{
		Update_Brush();

		if (iAnimFrame >= 43)
		{
			m_bBrushUpdate = false;
		}
	}

	if (m_bEffect == false && iAnimFrame >= 41)
	{
		Effect_Shot();
		m_bEffect = true;
	}

}

void CState_SP_Onestroke::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = &matWorld;
	EFFECT_START(L"OneStroke", &tDesc);
}

void CState_SP_Onestroke::Effect_Brush()
{
	CEffect_Manager::EFFECTPIVOTDESC tDesc;
	tDesc.pPivotMatrix = const_cast<Matrix*>(&dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix());
	EFFECT_START_OUTLIST(L"OneStrokeBrush", &tDesc, m_Effects);
}

void CState_SP_Onestroke::Update_Brush()
{
	Matrix matWorld = dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();
	m_Effects.front()->Update_Pivot(matWorld);
}

void CState_SP_Onestroke::Update_StartParticle()
{
	if (m_StartParticle.empty())
	{
		CEffect_Manager::EFFECTPIVOTDESC tDesc;
		tDesc.pPivotMatrix = const_cast<Matrix*>(&dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix());
		EFFECT_START_OUTLIST(L"OneStrokeStartParticle", &tDesc, m_StartParticle);
	}
	
	{
		Matrix matWorld = dynamic_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();
		for (auto& Particle : m_StartParticle)
		{
			Particle->Update_Pivot(matWorld);
		}
	}

}

void CState_SP_Onestroke::End_StartParticle()
{
	for (auto& Particle : m_StartParticle)
	{
		Particle->EffectEnd();
	}
	m_StartParticle.clear();
}

void CState_SP_Onestroke::Init_Camera()
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();
	m_vCameraTargetPos = m_pPlayer->Get_TargetPos();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_vCameraTargetPos - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();

	m_vRadialPos = vPos + vLook * 5.0f;

	Vec3 vTargetPos = vPos;
	vTargetPos.y += 0.7f;

	Vec3 vOffset = vLook * 1.0f + vRight * 0.5f + vUp * 1.0f;
	pCamera->Set_Mode(CCamera_Player::CameraState::FREE);
	pCamera->Set_TargetPos(vTargetPos);
	pCamera->Set_Offset(vOffset);
	pCamera->Set_CameraLength(3.0f);

	pCamera->ZoomInOut(1.0f, 3.0f);
}

void CState_SP_Onestroke::Update_Camera(_uint iAnimFrame, _float fTimeDelta)
{
	CCamera_Player* pCamera = m_pPlayer->Get_Camera();

	Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vLook = m_vCameraTargetPos - vPos;
	vLook.y = 0.0f;
	vLook.Normalize();
	Vec3 vUp(0.0f, 1.0f, 0.0f);
	Vec3 vRight = vUp.Cross(vLook);
	vRight.Normalize();

	if (iAnimFrame < 35)
	{
		Vec3 vTargetOffset = vRight * 1.0f + vLook * -0.5f + vUp * -0.1f;

		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = Vec3::Lerp(vOffset, vTargetOffset, 3.0f * fTimeDelta);
		pCamera->Set_Offset(vOffset);

		Vec3 vTargetPos = vPos;
		vTargetPos.y += 0.4f;
		Vec3 vCameraTargetPos = pCamera->Get_TargetPos();
		vTargetPos = Vec3::Lerp(vTargetPos, vCameraTargetPos, 3.0f * fTimeDelta);
		pCamera->Set_TargetPos(vTargetPos);
	}
	else
	{
		Vec3 vTargetOffset = vLook * 1.0f + vRight * 0.2f + vUp * 0.2f;
		Vec3 vOffset = pCamera->Get_Offset();
		vOffset = Vec3::Lerp(vOffset, vTargetOffset, 6.0f * fTimeDelta);
		pCamera->Set_Offset(vOffset);

		pCamera->ZoomInOut(10.0f, 10.0f);
	}
}

void CState_SP_Onestroke::Reset_Camera()
{
	m_pPlayer->Get_Camera()->Set_Mode(CCamera_Player::CameraState::DEFAULT);
	m_pPlayer->Get_Camera()->Set_DefaultOffset();
	m_pPlayer->Get_Camera()->DefaultLength(7.0f);
}

CState_SP_Onestroke* CState_SP_Onestroke::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Onestroke* pInstance = new CState_SP_Onestroke(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Onestroke");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Onestroke::Free()
{
	__super::Free();
}
