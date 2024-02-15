#include "stdafx.h"
#include "..\Public\State_SP_Identity_Sun.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "ServerSessionManager.h"
#include "Effect_Manager.h"
#include "Effect_Trail.h"
#include "Camera_Player.h"

CState_SP_Identity_Sun::CState_SP_Identity_Sun(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_Identity_Sun::Initialize()
{
	m_iIdentity_Sun = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_harmonyofyang", 1.0f);
	if (m_iIdentity_Sun == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_Identity_Sun::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_Identity_Sun::Tick_State_NoneControl;

	m_AttackFrames.push_back(20);
	m_AttackFrames.push_back(-1);


	// Sound
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_375.wav"), 0.3f)); // Player
	m_SoundFrames.push_back(SOUNDDESC(0, TEXT("Effect"), TEXT("SP_4.wav"))); // Skill
	m_SoundFrames.push_back(SOUNDDESC());

	return S_OK;
}

void CState_SP_Identity_Sun::Enter_State()
{
	m_EffectSound = false;
	m_PlayerSound = false;

	
	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].strGroup, m_SoundFrames[m_iSoundCnt].strName, m_SoundFrames[m_iSoundCnt].fVolume, true);
	CSound_Manager::GetInstance()->PlaySoundFile_AddChannel(m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].strGroup, m_SoundFrames[m_iSoundCnt + 1].strName, m_SoundFrames[m_iSoundCnt + 1].fVolume);


	m_iAttackCnt = 0;

	m_pPlayer->Reserve_Animation(m_iIdentity_Sun, 0.1f, 0, 0);

	m_pPlayer->Get_SP_Controller()->Get_StopMessage();
	m_pPlayer->Get_SP_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());

	Vec3 vDir = m_pPlayer->Get_TargetPos() - m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	if (3.f <= vDir.Length())
	{
		m_vColliPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vDir.Normalize();
		m_vColliPos += vDir * 4.f;
	}
	else
	{
		m_vColliPos = m_pPlayer->Get_TargetPos();
	}

	m_pPlayer->Get_SP_Controller()->Get_SP_IdentityMessage();
	m_pPlayer->Set_SuperArmorState(true);

	m_bTrail = false;
}

void CState_SP_Identity_Sun::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_Identity_Sun::Exit_State()
{
	m_pPlayer->Set_SuperArmorState(false);

	TrailEnd();

	if (true == m_pPlayer->Is_CancelState())
	{
		StopStateSound();
	}

}

void CState_SP_Identity_Sun::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iIdentity_Sun);

	if (m_bTrail == false)
	{
		Effect_Trail();
		m_bTrail = true;
	}

	if (m_AttackFrames[m_iAttackCnt] == iAnimFrame)
	{
		m_iAttackCnt++;
		
		Protocol::S_CREATE_SKILL pkt;
		pkt.set_ilevel(m_pPlayer->Get_CurrLevel());
		pkt.set_iplayerid(m_pPlayer->Get_ObjectID());
		pkt.set_iskillid(-1);
		pkt.set_iskilltype((_uint)SKILL_TYPE::SKILL_RISINGSUN);

		auto matWorld = pkt.mutable_matrix();
		matWorld->Resize(16, 0.0f);
		Matrix matPlayerWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		memcpy(matPlayerWorld.m[3], &m_vColliPos, sizeof(Vec3));
		memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

		CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iIdentity_Sun))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (35 <= iAnimFrame)
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

void CState_SP_Identity_Sun::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_SP_Identity_Sun::Effect_Trail()
{
	auto func = bind(&CPartObject::Load_Part_WorldMatrix, static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_1)), placeholders::_1);
	TRAIL_START_OUTLIST(TEXT("RisingSunTrail"), func, m_Trails);
}

void CState_SP_Identity_Sun::TrailEnd()
{
	for (auto& Trail : m_Trails)
	{
		dynamic_cast<CEffect_Trail*>(Trail)->TrailEnd(3.0f);
	}
}

CState_SP_Identity_Sun* CState_SP_Identity_Sun::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_Identity_Sun* pInstance = new CState_SP_Identity_Sun(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_Identity_Sun");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_Identity_Sun::Free()
{
	__super::Free();
}
