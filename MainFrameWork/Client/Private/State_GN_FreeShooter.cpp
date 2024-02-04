#include "stdafx.h"
#include "..\Public\State_GN_FreeShooter.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Camera_Player.h"

CState_GN_FreeShooter::CState_GN_FreeShooter(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_FreeShooter::Initialize()
{
	m_iFreeShoter = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_thefreeshooter", 1.2f);
	if (m_iFreeShoter == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_FreeShooter::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_FreeShooter::Tick_State_NoneControl;

	m_SkillFrames.push_back(8);
	m_SkillFrames.push_back(10);
	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(50);
	m_SkillFrames.push_back(63);
	m_SkillFrames.push_back(78);
	m_SkillFrames.push_back(100);

	m_SkillFrames.push_back(-1);


	m_EffectFrames.push_back(EFFECTFRAMEDESC(7, (_uint)CPartObject::PARTS::WEAPON_4));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(10, (_uint)CPartObject::PARTS::WEAPON_5));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(22, (_uint)CPartObject::PARTS::WEAPON_5));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(35, (_uint)CPartObject::PARTS::WEAPON_4));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(50, (_uint)CPartObject::PARTS::WEAPON_5));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(63, (_uint)CPartObject::PARTS::WEAPON_4));
	m_EffectFrames.push_back(EFFECTFRAMEDESC(78, (_uint)CPartObject::PARTS::WEAPON_5));

	m_EffectFrames.push_back(EFFECTFRAMEDESC());


	m_ParticleName.push_back(L"TerminateParticle1");
	m_ParticleName.push_back(L"TerminateParticle2");
	m_ParticleName.push_back(L"TerminateParticle3");
	m_ParticleName.push_back(L"TerminateParticle4");
	m_ParticleName.push_back(L"TerminateParticle5");
	m_ParticleName.push_back(L"TerminateParticle6");
	m_ParticleName.push_back(L"TerminateParticle7");

	return S_OK;
}

void CState_GN_FreeShooter::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iFreeShoter, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::SHOT, m_eSkillSelectKey);

	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, true);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	m_iEffectCnt = 0;

	m_bLastShotEffect = false;
}

void CState_GN_FreeShooter::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_FreeShooter::Exit_State()
{
	m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_FreeShooter::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		Effect_Shot();
		m_iEffectCnt++;
	}

	if (m_bLastShotEffect == false)
	{
		if (iAnimFrame == 100)
		{
			Effect_LastShot();
			m_bLastShotEffect = true;
		}
	}
	


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iFreeShoter))
		m_pPlayer->Set_State(TEXT("Idle"));

	if (110 <= iAnimFrame)
		m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	_uint iIdentity = static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity();

	if (true == m_pController->Is_Dash())
	{
		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
			m_pPlayer->Set_TargetPos(vClickPos);
		else
			m_pPlayer->Set_TargetPos(Vec3());

		m_pPlayer->Set_State(TEXT("Dash"));
	}
	else if (0 != static_cast<CPlayer_Controller_GN*>(m_pController)->Is_GN_Identity())
	{
		if (1 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN"));
		else if (2 == iIdentity)
			m_pPlayer->Set_State(TEXT("Identity_GN_Back"));
	}
}

void CState_GN_FreeShooter::Tick_State_NoneControl(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter);

	if (110 <= iAnimFrame)
		m_pPlayer->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_EffectFrames[m_iEffectCnt].iFrame <= iAnimFrame)
	{
		Effect_Shot();
		m_iEffectCnt++;
	}

	if (m_bLastShotEffect == false)
	{
		if (iAnimFrame == 100)
		{
			Effect_LastShot();
			m_bLastShotEffect = true;
		}
	}

}

void CState_GN_FreeShooter::Effect_Shot()
{
	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS(m_EffectFrames[m_iEffectCnt].iWeapon)))->Get_Part_WorldMatrix().Translation();
	matWorld.Translation(vPos);

	if (m_pPlayer->Is_Control())
	{
		Vec3 vRadialPos = matWorld.Translation() + matWorld.Backward() * 1.3f;
		vRadialPos.y += 0.7f;
		m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 500.f, 0.1f, 14.f);
		m_pPlayer->Get_Camera()->Set_RadialBlur(0.1f, vRadialPos, 0.2f, 0.03f);
	}

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("FreeShooterMuzzle"), &desc);

	for (_uint i = 0; i < 20; ++i)
	{
		//Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 50) * 0.02f);

		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);

		_uint iParticleNameIndex = rand() % 5;

		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
	}
}

void CState_GN_FreeShooter::Effect_LastShot()
{
	{
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_4))->Get_Part_WorldMatrix().Translation();
		matWorld.Translation(vPos);

		if (m_pPlayer->Is_Control())
		{
			Vec3 vRadialPos = matWorld.Translation() + matWorld.Backward() * 1.3f;
			vRadialPos.y += 0.7f;
			m_pPlayer->Get_Camera()->Cam_Shake(0.1f, 500.f, 0.1f, 14.f);
			m_pPlayer->Get_Camera()->Set_RadialBlur(0.1f, vRadialPos, 0.2f, 0.06f);
		}

		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("FreeShooterMuzzle"), &desc);

		for (_uint i = 0; i < 20; ++i)
		{
			//Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 50) * 0.02f);

			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);

			_uint iParticleNameIndex = rand() % 5;

			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			CEffect_Manager::EFFECTPIVOTDESC desc;
			Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
		}
	}


	{
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_5))->Get_Part_WorldMatrix().Translation();
		matWorld.Translation(vPos);

		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("FreeShooterMuzzle"), &desc);

		for (_uint i = 0; i < 20; ++i)
		{
			Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 50) * 0.02f);

			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.7f, 0.7f);

			_uint iParticleNameIndex = rand() % 5;

			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			CEffect_Manager::EFFECTPIVOTDESC desc;
			Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
		}
	}
}


CState_GN_FreeShooter* CState_GN_FreeShooter::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_FreeShooter* pInstance = new CState_GN_FreeShooter(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_FreeShooter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_FreeShooter::Free()
{
	__super::Free();
}
