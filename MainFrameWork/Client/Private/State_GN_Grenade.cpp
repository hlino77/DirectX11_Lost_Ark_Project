#include "stdafx.h"
#include "..\Public\State_GN_Grenade.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "GameInstance.h"
#include "Effect_Custom_Grenade.h"

CState_GN_Grenade::CState_GN_Grenade(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_Grenade::Initialize()
{
	m_iGrenade = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_at02grenade", 1.2f);
	if (m_iGrenade == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_Grenade::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_Grenade::Tick_State_NoneControl;

	m_SkillFrames.push_back(8);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_Grenade::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iGrenade, 0.1f, 0, 0, 0.6f);

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

void CState_GN_Grenade::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_Grenade::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_Grenade::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iGrenade);

	if (-1 != m_SkillFrames[m_iSkillCnt] && m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Shot();

		m_iSkillCnt++;
	}

	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iGrenade))
		m_pPlayer->Set_State(TEXT("Idle"));

}

void CState_GN_Grenade::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);
}

void CState_GN_Grenade::Effect_Shot()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Matrix matWorld = static_cast<CPartObject*>(m_pPlayer->Get_Parts(CPartObject::PARTS::WEAPON_2))->Get_Part_WorldMatrix();

	Vec3 vGunPos = matWorld.Translation();
	Vec3 vTargetPos = m_vColliPos;

	CEffect_Custom_Grenade::GrenadeDesc tDesc;
	tDesc.vPos = vGunPos;
	tDesc.vTargetPos = vTargetPos;
	tDesc.pPlayer = m_pPlayer;
	tDesc.iSkillKey = m_eSkillSelectKey;

	CGameObject* pObject = pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_Grenade", &tDesc);

	Safe_Release(pGameInstance);
}

CState_GN_Grenade* CState_GN_Grenade::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_Grenade* pInstance = new CState_GN_Grenade(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_Grenade");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_Grenade::Free()
{
	__super::Free();
}
