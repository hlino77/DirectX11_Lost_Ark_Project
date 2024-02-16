#include "stdafx.h"
#include "..\Public\State_SP_Grabbed.h"
#include "GameInstance.h"
#include "StateMachine.h"
#include "Player_Doaga.h"
#include "Controller_SP.h"
#include "Model.h"
#include "Boss_Valtan.h"
#include "BehaviorTree.h"
#include "BT_Action.h"


CState_Sp_Grabbed::CState_Sp_Grabbed(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
	: CState(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_Sp_Grabbed::Initialize()
{
	m_iGrabbed = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"ride_bubble_run_normal_1", 1.0f);
	if (m_iGrabbed == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_Sp_Grabbed::Tick_State_Control;
	else
		m_TickFunc = &CState_Sp_Grabbed::Tick_State_NoneControl;

	return S_OK;
}

void CState_Sp_Grabbed::Enter_State()
{
	m_pController->Get_StopMessage();
	m_pController->Get_SkillEndMessage();

	m_pPlayer->Reserve_Animation(m_iGrabbed, 0.1f, 0, 0, 0.f);

	m_pPlayer->Get_TransformCom()->Set_WorldMatrix(XMMatrixIdentity());
	m_pPlayer->Get_TransformCom()->My_Rotation(Vec3(90.f, 0.f, 0.f));
	m_pPlayer->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(-10.f, 40.f, 0.f));
	m_SaveMatrix = m_pPlayer->Get_TransformCom()->Get_WorldMatrix();

	m_pPlayer->Set_Navi(false);

	m_fGrabTimeAcc = 0.0f;
}

void CState_Sp_Grabbed::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_Sp_Grabbed::Exit_State()
{
	m_pController->Get_GrabEndMessage();

	m_pPlayer->Get_TransformCom()->Set_Up(Vec3(0.f, 1.f, 0.f));
	m_pPlayer->Set_Navi(true);

	m_pValtan = nullptr;
}

void CState_Sp_Grabbed::Tick_State_Control(_float fTimeDelta)
{
	To_GrabPos(fTimeDelta);
}

void CState_Sp_Grabbed::Tick_State_NoneControl(_float fTimeDelta)
{
	ToNone_GrabPos(fTimeDelta);

	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_Sp_Grabbed::To_GrabPos(_float fTimeDelta)
{
	Matrix Pivot = m_pController->Get_Grabber()->Get_ModelCom()->Get_PivotMatrix();
	XMMATRIX GrabMatrix = m_pController->Get_Grabber()->Get_ModelCom()->Get_CombinedMatrix(m_pController->Get_Grabber()->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand"))) * Pivot;

	GrabMatrix.r[0] = XMVector3Normalize(GrabMatrix.r[0]);
	GrabMatrix.r[1] = XMVector3Normalize(GrabMatrix.r[1]);
	GrabMatrix.r[2] = XMVector3Normalize(GrabMatrix.r[2]);

	Matrix WorldMatrix = m_SaveMatrix;
	WorldMatrix._41 *= 0.01f;
	WorldMatrix._42 *= 0.01f;
	WorldMatrix._43 *= 0.01f;
	WorldMatrix *= GrabMatrix;
	Matrix ComputeMatrix = WorldMatrix * m_pController->Get_Grabber()->Get_TransformCom()->Get_WorldMatrix();

	if (false == static_cast<CBoss_Valtan*>(m_pController->Get_Grabber())->Get_BehaviorTree()->Get_CurrentAction()->Get_CurrentAnimDesc().bIsLoop)
	{
		m_pPlayer->Get_TransformCom()->Set_WorldMatrix(ComputeMatrix);
		m_pPlayer->Get_TransformCom()->Set_Scale(Vec3(1.f, 1.f, 1.f));
	}
	else
	{
		m_fGrabTimeAcc += fTimeDelta;
		if (0.1f >= m_fGrabTimeAcc)
		{
			m_pPlayer->Get_TransformCom()->Set_WorldMatrix(ComputeMatrix);
			m_pPlayer->Get_TransformCom()->Set_Scale(Vec3(1.f, 1.f, 1.f));
		}
	}
}

void CState_Sp_Grabbed::ToNone_GrabPos(_float fTimeDelta)
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	if (nullptr == m_pValtan)
	{
		m_pValtan = pGameInstance->Find_GameObject(LEVELID::LEVEL_VALTANMAIN, (_uint)LAYER_TYPE::LAYER_BOSS, TEXT("Boss_Valtan"));
	}
		

	RELEASE_INSTANCE(CGameInstance);

	Matrix Pivot = m_pValtan->Get_ModelCom()->Get_PivotMatrix();
	XMMATRIX GrabMatrix = m_pValtan->Get_ModelCom()->Get_CombinedMatrix(m_pValtan->Get_ModelCom()->Find_BoneIndex(TEXT("bip001-l-hand"))) * Pivot;

	GrabMatrix.r[0] = XMVector3Normalize(GrabMatrix.r[0]);
	GrabMatrix.r[1] = XMVector3Normalize(GrabMatrix.r[1]);
	GrabMatrix.r[2] = XMVector3Normalize(GrabMatrix.r[2]);

	Matrix WorldMatrix = m_SaveMatrix;
	WorldMatrix._41 *= 0.01f;
	WorldMatrix._42 *= 0.01f;
	WorldMatrix._43 *= 0.01f;
	WorldMatrix *= GrabMatrix;
	Matrix ComputeMatrix = WorldMatrix * m_pValtan->Get_TransformCom()->Get_WorldMatrix();

	if (false == static_cast<CBoss_Valtan*>(m_pValtan)->Get_BehaviorTree()->Get_CurrentAction()->Get_CurrentAnimDesc().bIsLoop)
	{
		m_pPlayer->Get_TransformCom()->Set_WorldMatrix(ComputeMatrix);
		m_pPlayer->Get_TransformCom()->Set_Scale(Vec3(1.f, 1.f, 1.f));
	}
	else
	{
		m_fGrabTimeAcc += fTimeDelta;
		if (0.1f >= m_fGrabTimeAcc)
		{
			m_pPlayer->Get_TransformCom()->Set_WorldMatrix(ComputeMatrix);
			m_pPlayer->Get_TransformCom()->Set_Scale(Vec3(1.f, 1.f, 1.f));
		}
	}
}

CState_Sp_Grabbed* CState_Sp_Grabbed::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Doaga* pOwner)
{
	CState_Sp_Grabbed* pInstance = new CState_Sp_Grabbed(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_Sp_Grabbed");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_Sp_Grabbed::Free()
{
	__super::Free();
}
