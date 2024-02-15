#include "stdafx.h"
#include "..\Public\State_GN_PerfectShot_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Manager.h"
#include "GameInstance.h"
#include "Effect_Custom_PerpectShotBullet.h"
#include "Camera_Player.h"

CState_GN_PerfectShot_End::CState_GN_PerfectShot_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_PerfectShot_End::Initialize()
{
	m_iPerfectShot_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_perfectshot_success", 1.0f);
	if (m_iPerfectShot_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_PerfectShot_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_PerfectShot_End::Tick_State_NoneControl;


	m_SkillFrames.push_back(2);
	m_SkillFrames.push_back(-1);

	m_ParticleName.push_back(L"PerpectShotParticle1");
	m_ParticleName.push_back(L"PerpectShotParticle2");
	m_ParticleName.push_back(L"PerpectShotParticle3");
	m_ParticleName.push_back(L"PerpectShotParticle4");

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("GN_Perfect_190.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_GN_PerfectShot_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iPerfectShot_End, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_PerfectShot_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_PerfectShot_End::Exit_State()
{
	m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Set_SkillSuccess(false);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_GN_PerfectShot_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iPerfectShot_End);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (-1 != m_SoundFrames[m_iSoundCnt].iFrame && m_SoundFrames[m_iSoundCnt].iFrame <= (_int)iAnimFrame)
	{
		if (false == m_SoundFrames[m_iSoundCnt].bAddChannel)
		{
			CSound_Manager::GetInstance()->PlaySoundFile(m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}
		else
		{
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume);
		}

		m_iSoundCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd((_uint)m_iPerfectShot_End))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_PerfectShot_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_iPerfectShot_End))
	{
		Effect_Shot();

		m_iSkillCnt++;
	}
}

void CState_GN_PerfectShot_End::Effect_Shot()
{
	if (m_pPlayer->Is_Control())
		m_pPlayer->Get_Camera()->Cam_Shake(0.3f, 500.f, 0.2f, 12.f);

	Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
	Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_3))->Get_Part_WorldMatrix().Translation();

	Vec3 vOriginLook = matWorld.Backward();
	vOriginLook.Normalize();

	Vec3 vOriginUp = matWorld.Up();
	vOriginUp.Normalize();

	Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
	vOriginRight.Normalize();

	matWorld.Translation(vPos);


	CEffect_Custom_PerpectShotBullet::PerpectShotBulletDesc tDesc;
	tDesc.vPos = vPos;
	tDesc.vLook = vOriginLook;

	CGameObject* pObject = CGameInstance::GetInstance()->Add_GameObject(CGameInstance::GetInstance()->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_PerpectShotBullet", &tDesc);

	CEffect_Manager::EFFECTPIVOTDESC desc;
	desc.pPivotMatrix = &matWorld;
	EFFECT_START(TEXT("PerpectShotSmoke"), &desc)
	EFFECT_START(TEXT("PerpectShotMuzzleFlash"), &desc) 
	
	for (_uint i = 0; i < 60; ++i)
	{
		Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 10) * 0.1f);

		_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.5f, 0.5f);
		_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.5f, 0.5f);

		_uint iParticleNameIndex = rand() % 4;

		Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matEffectWorld = Matrix::CreateWorld(vRandomPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
		desc.pPivotMatrix = &matEffectWorld;

		EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
	}
}

CState_GN_PerfectShot_End* CState_GN_PerfectShot_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_PerfectShot_End* pInstance = new CState_GN_PerfectShot_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_PerfectShot_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_PerfectShot_End::Free()
{
	__super::Free();
}
