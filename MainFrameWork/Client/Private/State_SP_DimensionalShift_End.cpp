#include "stdafx.h"
#include "..\Public\State_SP_DimensionalShift_End.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Player_Skill.h"
#include "Model.h"
#include "ServerSessionManager.h"

CState_SP_DimensionalShift_End::CState_SP_DimensionalShift_End(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_SP_DimensionalShift_End::Initialize()
{
	m_iDimensionalShift_End = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_doodle_03", 1.0f);
	if (m_iDimensionalShift_End == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_SP_DimensionalShift_End::Tick_State_Control;
	else
		m_TickFunc = &CState_SP_DimensionalShift_End::Tick_State_NoneControl;

	m_SkillFrames.push_back(20);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_SP_DimensionalShift_End::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iDimensionalShift_End, 0.1f, 0, 0);

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_SP_DimensionalShift_End::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_SP_DimensionalShift_End::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_SP_DimensionalShift_End::Tick_State_Control(_float fTimeDelta)
{
	if (m_SkillFrames[m_iSkillCnt] == m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDimensionalShift_End))
	{
		m_iSkillCnt++;
		
		Protocol::S_CREATE_SKILL pkt;
		pkt.set_ilevel(m_pPlayer->Get_CurrLevel());
		pkt.set_iplayerid(m_pPlayer->Get_ObjectID());
		pkt.set_iskillid(-1);
		pkt.set_iskilltype((_uint)SKILL_TYPE::SKILL_TELEPORTDOOR);

		auto matWorld = pkt.mutable_matrix();
		matWorld->Resize(16, 0.0f);
		Matrix matPlayerWorld = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
		Vec3 vPos = m_pPlayer->Get_TargetPos();
		vPos.y += 0.7f;
		memcpy(matPlayerWorld.m[3], &vPos, sizeof(Vec3));
		memcpy(matWorld->mutable_data(), &matPlayerWorld, sizeof(Matrix));

		CServerSessionManager::GetInstance()->Send(CClientPacketHandler::MakeSendBuffer(pkt));
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDimensionalShift_End))
		m_pPlayer->Set_State(TEXT("Idle"));


	if (30 <= m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDimensionalShift_End))
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

void CState_SP_DimensionalShift_End::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_SP_DimensionalShift_End* CState_SP_DimensionalShift_End::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_SP_DimensionalShift_End* pInstance = new CState_SP_DimensionalShift_End(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_SP_DimensionalShift_End");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_SP_DimensionalShift_End::Free()
{
	__super::Free();
}
