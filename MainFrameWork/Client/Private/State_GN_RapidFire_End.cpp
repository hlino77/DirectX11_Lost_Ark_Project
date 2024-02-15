#include "stdafx.h"
#include "..\Public\State_GN_RapidFire_End.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Manager.h"


CState_GN_RapidFire_End::CState_GN_RapidFire_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_RapidFire_End::Initialize()
{
	m_iRapidFire_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_rapidfire", 1.2f);
	if (m_iRapidFire_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_RapidFire_End::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_RapidFire_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(5);
	m_SkillFrames.push_back(8);

	m_SkillFrames.push_back(10);

	m_SkillFrames.push_back(16);
	m_SkillFrames.push_back(19);

	m_SkillFrames.push_back(21);

	m_SkillFrames.push_back(-1);

	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("GN_Rapidfire_201.wav")));
	m_SoundFrames.push_back(SOUNDDESC(27, TEXT("Effect"), TEXT("GN_Laugh_598.wav")));
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_GN_RapidFire_End::Enter_State()
{
	m_iSkillCnt = 0;
	m_iSoundCnt = 0;

	m_pPlayer->Reserve_Animation(m_iRapidFire_End, 0.2f, 2, 0, 1.8f);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_RapidFire_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_RapidFire_End::Exit_State()
{
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);

	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_GN_RapidFire_End::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame((m_iRapidFire_End));

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
			CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
		}

		m_iSoundCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iRapidFire_End))
		m_pPlayer->Set_State(TEXT("Idle"));

}

void CState_GN_RapidFire_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)m_pPlayer->Get_ModelCom()->Get_Anim_Frame((_uint)m_iRapidFire_End))
	{
		Effect_Shot();

		m_iSkillCnt++;
	}
}

void CState_GN_RapidFire_End::Effect_Shot()
{
	{
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix().Translation();

		Vec3 vOriginLook = vPos - matWorld.Translation();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(vOriginLook);
		vOriginRight.Normalize();

		vOriginLook = vOriginRight.Cross(vOriginUp);
		vOriginLook.Normalize();

		matWorld = Matrix::CreateWorld(vPos, -vOriginLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

		_uint iCount = rand() % 2 + 1;
		for (_uint i = 0; i < iCount; ++i)
		{
			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

			CEffect_Manager::EFFECTPIVOTDESC desc;
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(TEXT("DeathFireBulletParticle"), &desc)
		}
	}

	{
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_2))->Get_Part_WorldMatrix().Translation();

		Vec3 vOriginLook = vPos - matWorld.Translation();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(vOriginLook);
		vOriginRight.Normalize();

		vOriginLook = vOriginRight.Cross(vOriginUp);
		vOriginLook.Normalize();

		matWorld = Matrix::CreateWorld(vPos, -vOriginLook, Vec3(0.0f, 1.0f, 0.0f));

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

			_uint iCount = rand() % 2 + 1;
		for (_uint i = 0; i < iCount; ++i)
		{
			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.1f, 0.1f);


			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			Matrix matEffectWorld = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

			CEffect_Manager::EFFECTPIVOTDESC desc;
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(TEXT("DeathFireBulletParticle"), &desc)
		}
	}
}

CState_GN_RapidFire_End* CState_GN_RapidFire_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_RapidFire_End* pInstance = new CState_GN_RapidFire_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_RapidFire_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_RapidFire_End::Free()
{
	__super::Free();
}
