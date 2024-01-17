#include "stdafx.h"
#include "..\Public\State_GN_Grabbed.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CState_GN_Grabbed::CState_GN_Grabbed(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Grabbed::Initialize()
{
	m_iGrabbed = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"ride_bubble_run_normal_1", 1.0f);
	if (m_iGrabbed == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Grabbed::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Grabbed::Tick_State_NoneControl;

	return S_OK;
}

void CState_GN_Grabbed::Enter_State()
{
	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();

	m_pPlayer->Get_TransformCom()->Set_Scale(Vec3(100.f, 100.f, 100.f));
	m_pPlayer->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(0.f, 0.f, 0.f));
	m_SaveMatrix = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();
}

void CState_GN_Grabbed::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Grabbed::Exit_State()
{
	m_pController->Get_GrabEndMessage();
}

void CState_GN_Grabbed::Tick_State_Control(_float fTimeDelta)
{
	To_GrabPos(fTimeDelta);
}

void CState_GN_Grabbed::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_Grabbed::To_GrabPos(_float fTimeDelta)
{
	Matrix GrabMatrix = m_pController->Get_Grabber()->Get_ModelCom()->Get_CombinedMatrix(m_pController->Get_Grabber()->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand")));

	m_pPlayer->Get_TransformCom()->Set_WorldMatrix(m_SaveMatrix * GrabMatrix);
}

CState_GN_Grabbed* CState_GN_Grabbed::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Grabbed* pInstance = new CState_GN_Grabbed(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Grabbed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Grabbed::Free()
{
	__super::Free();
}
