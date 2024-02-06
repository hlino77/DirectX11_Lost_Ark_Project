#include "stdafx.h"
#include "..\Public\State_GN_FocusShot_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "Effect.h"
#include "Camera_Player.h"

CState_GN_FocusShot_End::CState_GN_FocusShot_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_FocusShot_End::Initialize()
{
	m_iFocuseShot_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_03", 1.0f);
	if (m_iFocuseShot_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_FocusShot_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_FocusShot_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(10);

	m_SkillFrames.push_back(-1);

	m_ParticleName.push_back(L"FocusShotParticle1");
	m_ParticleName.push_back(L"FocusShotParticle2");
	m_ParticleName.push_back(L"FocusShotParticle3");
	m_ParticleName.push_back(L"FocusShotParticle4");
	m_ParticleName.push_back(L"FocusShotParticle5");

	return S_OK;
}

void CState_GN_FocusShot_End::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFocuseShot_End, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_bEffect = false;
}

void CState_GN_FocusShot_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_FocusShot_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pController->Is_HitState())
	{
		Effect_Glow(false);
	}
}

void CState_GN_FocusShot_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFocuseShot_End);

	if ((_int)iAnimFrame < m_SkillFrames[m_iSkillCnt])
	{
		Update_Effect(fTimeDelta);
	}
	
	if (m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Glow(false);
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFocuseShot_End))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
}

void CState_GN_FocusShot_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);


	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFocuseShot_End);

	if ((_int)iAnimFrame < m_SkillFrames[m_iSkillCnt])
	{
		Update_Effect(fTimeDelta);
	}

	if (m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Glow(false);
		Effect_Shot();

		m_iSkillCnt++;
	}

}

void CState_GN_FocusShot_End::Effect_Glow(_bool bOnOff)
{
	if (bOnOff)
	{
		vector<CEffect*> EffectList;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = const_cast<Matrix*>(&static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix());
		EFFECT_START_OUTLIST(TEXT("LongLazer"), &desc, EffectList);
		EFFECT_START_OUTLIST(TEXT("FocusShotGlow2"), &desc, EffectList);

		m_pEffectGlow = EffectList[0];
		m_pEffectLazer = EffectList[1];
	}
	else
	{
		if (m_pEffectGlow)
		{
			m_pEffectGlow->EffectEnd();
			m_pEffectGlow = nullptr;
		}

		if (m_pEffectLazer)
		{
			m_pEffectLazer->EffectEnd();
			m_pEffectLazer = nullptr;
		}
	}
}

void CState_GN_FocusShot_End::Update_Effect(_float fTimeDelta)
{
	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix();

	if (m_bEffect == false)
	{
		Effect_Glow(true);
		m_bEffect = true;
	}


	if (m_pEffectGlow)
		m_pEffectGlow->Update_Pivot(matWorld);

	if (m_pEffectLazer)
		m_pEffectLazer->Update_Pivot(matWorld);
}

void CState_GN_FocusShot_End::Effect_Shot()
{
	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.3f, 500.f, 0.2f, 12.f);

	CEffect_Manager::EFFECTPIVOTDESC desc;
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("FocusShotBullet2"), &desc)
	EFFECT_START(TEXT("FocusShotMuzzleFlash2"), &desc)
	EFFECT_START(TEXT("FocusShotSmoke2"), &desc);

	for (_uint i = 0; i < 50; ++i)
	{
		Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 50) * 0.02f + 0.4f);

		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.3f, 0.3f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.3f, 0.3f);

		_uint iParticleNameIndex = rand() % 5;

		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matEffectWorld = Matrix::CreateWorld(vRandomPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
	}
}

CState_GN_FocusShot_End* CState_GN_FocusShot_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_FocusShot_End* pInstance = new CState_GN_FocusShot_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_FocusShot_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_FocusShot_End::Free()
{
	__super::Free();
}
