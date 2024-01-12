#include "stdafx.h"
#include "..\Public\State_GN_SprialChaser.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Custom_SpiralChaser.h"


CState_GN_SprialChaser::CState_GN_SprialChaser(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_SprialChaser::Initialize()
{
	m_iSprialChaser = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_spiralchaser", 1.2f);
	if (m_iSprialChaser == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_SprialChaser::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_SprialChaser::Tick_State_NoneControl;


	m_SkillFrames.push_back(8);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_SprialChaser::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iSprialChaser, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();
	m_pPlayer->Get_GN_Controller()->Get_LerpDirLookMessage(m_pPlayer->Get_TargetPos());
	m_pPlayer->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eSkillSelectKey);
	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());

	Vec3 vDir = m_pPlayer->Get_TargetPos() - m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	if (5.f <= vDir.Length())
	{
		m_vColliPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
		vDir.Normalize();
		m_vColliPos += vDir * 5.f;
	}
	else
	{
		m_vColliPos = m_pPlayer->Get_TargetPos();
	}
}

void CState_GN_SprialChaser::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_SprialChaser::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_SprialChaser::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iSprialChaser);

	if (m_SkillFrames[m_iSkillCnt] == iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iSprialChaser))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_SprialChaser::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_SprialChaser::Effect_Shot()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_2))->Get_Part_WorldMatrix();

	Vec3 vGunPos = matWorld.Translation();
	Vec3 vTargetPos = m_vColliPos;
	vTargetPos.y += 0.7f;

	CEffect_Custom_SpiralChaser::SpiralChaserDesc tDesc;
	tDesc.vPos = vGunPos;
	tDesc.vTargetPos = vTargetPos;
	tDesc.pPlayer = m_pPlayer;
	tDesc.iSkillKey = m_eSkillSelectKey;

	CGameObject* pObject = pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_SpiralChaser", &tDesc);

	Safe_Release(pGameInstance);
}

CState_GN_SprialChaser* CState_GN_SprialChaser::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_SprialChaser* pInstance = new CState_GN_SprialChaser(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_SprialChaser");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_SprialChaser::Free()
{
	__super::Free();
}
