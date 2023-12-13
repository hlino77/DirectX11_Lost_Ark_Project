#include "stdafx.h"
#include "..\Public\State_GN_Idle.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Idle::CState_GN_Idle(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
    : CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Idle::Initialize()
{
	m_iIdle = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"idle_identity3", 1.0f);
	m_pPlayer->Get_ModelCom()->Set_CurrAnim(m_iIdle);
	if (m_iIdle == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Idle::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Idle::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Idle::Enter_State()
{
	m_pPlayer->Reserve_Animation(m_iIdle, 0.1f, 0, 0);
}

void CState_GN_Idle::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

wstring CState_GN_Idle::LateTick_State(_float fTimeDelta)
{
	if (m_strChangeName != m_strStateName)
		return m_strChangeName;
	else
		return m_strStateName;
}

void CState_GN_Idle::Exit_State()
{

}

void CState_GN_Idle::Tick_State_Control(_float fTimeDelta)
{
	if (true == m_pController->Is_Run())
	{
		Vec3 vPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);

		Vec3 vClickPos;
		if (true == m_pPlayer->Get_CellPickingPos(vClickPos))
		{
			if(Vec3(vClickPos - vPos).Length() <= 0.03f)
				m_strChangeName = m_strStateName;
			else
			{
				m_pController->Get_MoveMessage(vClickPos);

				m_strChangeName = TEXT("GN_Run");
			}

		}
		else
			m_strChangeName = m_strStateName;
	}
	else
		m_strChangeName = m_strStateName;
}

void CState_GN_Idle::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

CState_GN_Idle* CState_GN_Idle::Create( wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Idle* pInstance = new CState_GN_Idle(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Idle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Idle::Free()
{
	__super::Free();
}
