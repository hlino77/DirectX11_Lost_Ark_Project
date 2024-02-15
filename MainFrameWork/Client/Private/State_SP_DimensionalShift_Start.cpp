#include "stdafx.h"
#include "..\Public\State_SP_DimensionalShift_Start.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Manager.h"

CState_SP_DimensionalShift_Start::CState_SP_DimensionalShift_Start(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_DimensionalShift_Start::Initialize()
{
	m_iDimensionalShift_Start = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_doodle_01", 1.0f);
	if (m_iDimensionalShift_Start == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_DimensionalShift_Start::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_DimensionalShift_Start::Tick_State_NoneControl;

	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_246.wav"), 0.3f)); // Player 
	m_SoundFrames.push_back(SOUNDDESC());


	return S_OK;
}

void CState_SP_DimensionalShift_Start::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	
	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);




	m_pPlayer->Reserve_Animation(m_iDimensionalShift_Start, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	m_pPlayer->Get_SP_Controller()->Get_SkillMessage(m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	Vec3 vDir = m_pPlayer->Get_TargetPos() - m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	if (4.f <= vDir.Length())
	{
		m_vColliPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vDir.Normalize();
		m_vColliPos += vDir * 4.f;
		m_pPlayer->Set_TargetPos(m_vColliPos);
	}

	m_bTrail = false;
}

void CState_SP_DimensionalShift_Start::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_DimensionalShift_Start::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);

	if (m_pPlayer->Get_ServerState() != L"Skill_SP_DimensionalShift_End")
	{
		TrailEnd();
	}

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}
}

void CState_SP_DimensionalShift_Start::Tick_State_Control(_float fTimeDelta)
{
	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDimensionalShift_Start))
		m_pPlayer->Set_State(TEXT("Skill_SP_DimensionalShift_End"));
}

void CState_SP_DimensionalShift_Start::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}
}

void CState_SP_DimensionalShift_Start::Effect_Trail()
{
	vector<CEffect*> Trails;
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("TeleportDoorTrail"), func, Trails);

	m_pPlayer->Add_Effect(L"TeleportDoorTrail1", Trails[0]);
	m_pPlayer->Add_Effect(L"TeleportDoorTrail2", Trails[1]);
}

void CState_SP_DimensionalShift_Start::TrailEnd()
{
	m_pPlayer->Delete_Effect_Trail(L"TeleportDoorTrail1", 3.0f);
	m_pPlayer->Delete_Effect_Trail(L"TeleportDoorTrail2", 3.0f);
}

CState_SP_DimensionalShift_Start* CState_SP_DimensionalShift_Start::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_DimensionalShift_Start* pInstance = new CState_SP_DimensionalShift_Start(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_DimensionalShift_Start");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_DimensionalShift_Start::Free()
{
	__super::Free();
}
