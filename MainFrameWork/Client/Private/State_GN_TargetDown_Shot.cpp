#include "stdafx.h"
#include "..\Public\State_GN_TargetDown_Shot.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Camera_Player.h"
#include "Effect_Custom_CrossHair.h"

CState_GN_TargetDown_Shot::CState_GN_TargetDown_Shot(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_TargetDown_Shot::Initialize()
{
	m_iTargetDown_Shot = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_targetdown_03", 1.0f);
	if (m_iTargetDown_Shot == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_TargetDown_Shot::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_TargetDown_Shot::Tick_State_NoneControl;

	m_SkillFrames.push_back(1);

	m_SkillFrames.push_back(-1);


	m_ParticleName.push_back(L"TargetDownParticle1");
	m_ParticleName.push_back(L"TargetDownParticle2");
	m_ParticleName.push_back(L"TargetDownParticle3");
	m_ParticleName.push_back(L"TargetDownParticle4");

	m_ParticleName.push_back(L"FocusShotParticle1");
	m_ParticleName.push_back(L"FocusShotParticle2");
	m_ParticleName.push_back(L"FocusShotParticle3");
	m_ParticleName.push_back(L"FocusShotParticle4");
	m_ParticleName.push_back(L"FocusShotParticle5");

	return S_OK;
}

void CState_GN_TargetDown_Shot::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iTargetDown_Shot, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();

	m_bEffect = false;
}

void CState_GN_TargetDown_Shot::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_TargetDown_Shot::Exit_State()
{
	if (true == m_pController->Is_HitState())
	{
		//Effect_End();
	}
}

void CState_GN_TargetDown_Shot::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iTargetDown_Shot))
	{
		m_iSkillCnt++;
		Effect_Shot();
		static_cast<CPlayer_Controller_GN*>(m_pController)->Get_SkillAttackMessage(m_eSkillSelectKey, m_pPlayer->Get_TargetPos());
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iTargetDown_Shot))
	{
		m_pPlayer->Set_State(TEXT("Skill_GN_TargetDown_Loop"));
	}
}

void CState_GN_TargetDown_Shot::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_TargetDown_Shot::Effect_Shot()
{
	{
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
		EFFECT_START(TEXT("FocusShotMuzzleFlash1"), &desc);

		for (_uint i = 0; i < 30; ++i)
		{
			Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 30) * 0.02f + 0.4f);

			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-0.2f, 0.2f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-0.2f, 0.2f);

			_uint iParticleNameIndex = rand() % 5 + 4;

			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			CEffect_Manager::EFFECTPIVOTDESC desc;
			Matrix matEffectWorld = Matrix::CreateWorld(vRandomPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
			desc.pPivotMatrix = &matWorld;

			EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
		}
	}
	
	{
		CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

		

		if (m_pPlayer->Is_Control())
		{
			m_pPlayer->Get_Camera()->Cam_Shake(0.7f, 108.f, 0.3f, 18.0f);

			CEffect_Custom_CrossHair* pEffect = dynamic_cast<CEffect_Custom_CrossHair*>(pGameInstance->Find_GameObejct(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"GN_CrossHair"));

			pEffect->EffectShot();
		}
			
		Vec3 vPlayerPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		Vec3 vPos = m_pPlayer->Get_TargetPos();
		vPos.y += 0.7f;

		Vec3 vOriginLook = vPos - vPlayerPos;
		vOriginLook.y = 0.0f;
		vOriginLook.Normalize();

		vPos -= vOriginLook * 0.5f;

		Vec3 vOriginUp(Vec3(0.0f, 1.0f, 0.0f));

		Vec3 vOriginRight = vOriginUp.Cross(vOriginLook);
		vOriginRight.Normalize();

		Matrix matWorld = Matrix::CreateWorld(vPos, -vOriginLook, vOriginUp);


		CEffect_Manager::EFFECTPIVOTDESC desc;
		desc.pPivotMatrix = &matWorld;
		EFFECT_START(TEXT("TargetDown"), &desc);

		for (_uint i = 0; i < 60; ++i)
		{
			Vec3 vRandomPos = vPos + vOriginLook * ((rand() % 10) * 0.1f - 0.5f);

			_float fRandomY = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);
			_float fRandomX = CGameInstance::GetInstance()->Get_RandomFloat(-1.0f, 1.0f);

			_uint iParticleNameIndex = rand() % 4;

			Vec3 vLook = vOriginLook + vOriginUp * fRandomY + vOriginRight * fRandomX;

			CEffect_Manager::EFFECTPIVOTDESC desc;
			Matrix matEffectWorld = Matrix::CreateWorld(vRandomPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));
			desc.pPivotMatrix = &matEffectWorld;

			EFFECT_START(m_ParticleName[iParticleNameIndex], &desc)
		}

		Safe_Release(pGameInstance);
	}

	
}

void CState_GN_TargetDown_Shot::Effect_End()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CEffect_Custom_CrossHair* pEffect = dynamic_cast<CEffect_Custom_CrossHair*>(pGameInstance->Find_GameObejct(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"GN_CrossHair"));

	pEffect->EffectEnd();

	m_pPlayer->Get_Camera()->DefaultLength(7.0f);


	CEffect* pDecal = dynamic_cast<CEffect*>(pGameInstance->Find_GameObejct(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, L"Effect_TargetDownDecal"));
	pDecal->Set_ObjectTag(L"Effect_TargetDownDecalReady");
	pDecal->EffectEnd();

	Safe_Release(pGameInstance);
}

CState_GN_TargetDown_Shot* CState_GN_TargetDown_Shot::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_TargetDown_Shot* pInstance = new CState_GN_TargetDown_Shot(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_TargetDown_Shot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_TargetDown_Shot::Free()
{
	__super::Free();
}
