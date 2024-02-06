#include "stdafx.h"
#include "..\Public\State_GN_DeathFire_Success.h"
#include "StateMachine.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Player_Skill.h"
#include "Model.h"
#include "Effect_Custom_DeathFireBomb.h"
#include "GameInstance.h"

CState_GN_DeathFire_Success::CState_GN_DeathFire_Success(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
	: CState_Skill(strStateName, pMachine, pController), m_pPlayer(pOwner)
{
}

HRESULT CState_GN_DeathFire_Success::Initialize()
{
	m_iDeathFire_Success = m_pPlayer->Get_ModelCom()->Initailize_FindAnimation(L"sk_deathfire_success", 1.2f);
	if (m_iDeathFire_Success == -1)
		return E_FAIL;

	if (m_pPlayer->Is_Control())
		m_TickFunc = &CState_GN_DeathFire_Success::Tick_State_Control;
	else
		m_TickFunc = &CState_GN_DeathFire_Success::Tick_State_NoneControl;

	m_SkillFrames.push_back(12);
	m_SkillFrames.push_back(-1);

	return S_OK;
}

void CState_GN_DeathFire_Success::Enter_State()
{
	m_iSkillCnt = 0;

	m_pPlayer->Reserve_Animation(m_iDeathFire_Success, 0.1f, 0, 0);

	m_pPlayer->Get_GN_Controller()->Get_StopMessage();

	m_vEffectPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	m_bEffect = false;

	m_pPlayer->Set_SuperArmorState(m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor());
}

void CState_GN_DeathFire_Success::Tick_State(_float fTimeDelta)
{
	m_TickFunc(*this, fTimeDelta);
}

void CState_GN_DeathFire_Success::Exit_State()
{
	if (true == m_pController->Get_PlayerSkill(m_eSkillSelectKey)->Is_SuperArmor())
		m_pPlayer->Set_SuperArmorState(false);
}

void CState_GN_DeathFire_Success::Tick_State_Control(_float fTimeDelta)
{
	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeathFire_Success);

	if (m_SkillFrames[m_iSkillCnt] <= iAnimFrame)
	{
		Effect_Bomb();

		m_iSkillCnt++;
	}


	if (true == m_pPlayer->Get_ModelCom()->Is_AnimationEnd(m_iDeathFire_Success))
		m_pPlayer->Set_State(TEXT("Idle"));
}

void CState_GN_DeathFire_Success::Tick_State_NoneControl(_float fTimeDelta)
{
	m_pPlayer->Follow_ServerPos(0.01f, 6.0f * fTimeDelta);

	_uint iAnimFrame = m_pPlayer->Get_ModelCom()->Get_Anim_Frame(m_iDeathFire_Success);

	if (m_SkillFrames[m_iSkillCnt] <= (_int)iAnimFrame)
	{
		Effect_Bomb();

		m_iSkillCnt++;
	}
}

void CState_GN_DeathFire_Success::Effect_Bomb()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	Vec3 vLook = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_LOOK);
	vLook.Normalize();
	Vec3 vTargetPos = m_pPlayer->Get_TransformCom()->Get_State(CTransform::STATE_POSITION);
	Vec3 vStartPos = vTargetPos;
	vStartPos.y += 1.0f;

	for (_uint i = 0; i < 8; ++i)
	{
		CEffect_Custom_DeathFireBomb::DeathFireBombDesc tDesc;
		tDesc.vPos = vStartPos;
		tDesc.vTargetPos = vTargetPos + vLook * 2.0f;
		tDesc.pPlayer = m_pPlayer;
		tDesc.iSkillKey = m_eSkillSelectKey;

		if (i == 0)
			tDesc.bFirst = true;
		else
			tDesc.bFirst = false;

		CGameObject* pObject = pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_DeathFireBomb", &tDesc);

		vLook = XMVector3Rotate(vLook, Quaternion::CreateFromAxisAngle(Vec3(0.0f, 1.0f, 0.0f), XMConvertToRadians(45.0f)));
	}

	Safe_Release(pGameInstance);

}

CState_GN_DeathFire_Success* CState_GN_DeathFire_Success::Create(wstring strStateName, CStateMachine* pMachine, CPlayer_Controller* pController, CPlayer_Gunslinger* pOwner)
{
	CState_GN_DeathFire_Success* pInstance = new CState_GN_DeathFire_Success(strStateName, pMachine, pController, pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Cloned : CState_GN_DeathFire_Success");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CState_GN_DeathFire_Success::Free()
{
	__super::Free();
}
