#include "stdafx.h"
#include "..\Public\State_GN_DeathFire_Start.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"
#include "GameInstance.h"

CState_GN_DeathFire_Start::CState_GN_DeathFire_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_DeathFire_Start::Initialize()
{
	m_iDeathFire_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deathfire_start", 1.2f);
	if (m_iDeathFire_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_DeathFire_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_DeathFire_Start::Tick_State_NoneControl;

	m_SkillFrames.push_back(22);
	m_SkillFrames.push_back(25);
	m_SkillFrames.push_back(28);

	m_SkillFrames.push_back(32);
	m_SkillFrames.push_back(35);
	m_SkillFrames.push_back(38);

	m_SkillFrames.push_back(42);
	m_SkillFrames.push_back(45);
	m_SkillFrames.push_back(48);
	
	m_SkillFrames.push_back(52);
	m_SkillFrames.push_back(55);
	m_SkillFrames.push_back(58);

	m_SkillFrames.push_back(62);
	m_SkillFrames.push_back(65);
	m_SkillFrames.push_back(68);

	m_SkillFrames.push_back(72);
	m_SkillFrames.push_back(75);
	m_SkillFrames.push_back(78);

	m_SkillFrames.push_back(92);
	m_SkillFrames.push_back(95);
	m_SkillFrames.push_back(98);

	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_DeathFire_Start::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iDeathFire_Start, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_DeathFire_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_DeathFire_Start::Exit_State()
{
}

void CState_GN_DeathFire_Start::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeathFire_Start);

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		if (iAnimFrame < 90)
			Effect_Shot();
		else
			Effect_Shot_Right();

		Effect_RandomShot();

		m_iSkillCnt++;
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey);
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDeathFire_Start))
		m_pPlayer->Set_State(TEXT("Skill_GN_DeathFire_Success"));
	else if (true == m_pController->Is_Away(m_eSkillBindKey))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_DeathFire_Success"));
	}
}

void CState_GN_DeathFire_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_DeathFire_Start::Effect_Shot()
{
	{
		Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

		Vec3 vPos = matWorld.Translation();

		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

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
		Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_2))->Get_Part_WorldMatrix();

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

		Vec3 vPos = matWorld.Translation();

		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

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

void CState_GN_DeathFire_Start::Effect_Shot_Right()
{
	{
		Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1))->Get_Part_WorldMatrix();

		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("DeathFireBullet"), &desc)

			Vec3 vPos = matWorld.Translation();

		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

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

void CState_GN_DeathFire_Start::Effect_RandomShot()
{
	{
		CEffect_Manager::EFFECTPIVOTDESC desc;
		Matrix matWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		_float fRandomAngle = XMConvertToRadians((rand() % 360) * 1.0f);

		matWorld *= Matrix::CreateFromAxisAngle(m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_UP), fRandomAngle);
		Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vOriginLook = matWorld.Backward();
		vOriginLook.Normalize();
		vPos.y += 0.7f;
		vPos += vOriginLook * 0.7f;

		Vec3 vOriginUp = matWorld.Up();
		vOriginUp.Normalize();

		Vec3 vOriginRight = vOriginUp.Cross(matWorld.Backward());
		vOriginRight.Normalize();

		matWorld.Translation(vPos);

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

CState_GN_DeathFire_Start* CState_GN_DeathFire_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_DeathFire_Start* pInstance = new CState_GN_DeathFire_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_DeathFire_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_DeathFire_Start::Free()
{
	__super::Free();
}
