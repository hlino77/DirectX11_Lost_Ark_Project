#include "..\Public\Skill_GN_FocusShot.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CSkill_GN_FocusShot::CSkill_GN_FocusShot(CGameObject* pOwner)
	: CPlayer_Skill(pOwner)
{
}

HRESULT CSkill_GN_FocusShot::Initialize()
{
	/* ¼¦°Ç½ºÅ³ */
	m_eAttackType = { SKILL_ATTACKTYPE::NORMAL };
	m_eCtrlType = { SKILL_CTRLTYPE::NORMAL };

	m_fSkillDamage = { 0.f };
	m_fSkillCoolTime = 8.f;

	m_iFocusShot_1 = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_01", 1.2f);
	if (m_iFocusShot_1 == -1)
		return E_FAIL;

	m_iFocusShot_2 = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_02", 1.0f);
	if (m_iFocusShot_2 == -1)
		return E_FAIL;

	m_iFocusShot_3 = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_focusshot_03", 1.0f);
	if (m_iFocusShot_3 == -1)
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_FocusShot::Enter()
{
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_1, 0.1f, 0, 0);
	m_bAnimEnd[0] = false;

	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_StopMessage();
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_LerpLookMessage(static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_TargetPos());
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::LONG, m_eBindCtrlKey);
}

void CSkill_GN_FocusShot::Tick(_float fTimeDelta)
{
	if (false == m_bAnimEnd[0] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_1))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_2, 0.1f, 0, 0);
		m_bAnimEnd[0] = true; 
		m_bAnimEnd[1] = false;
	}
	else if (false == m_bAnimEnd[1] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_2))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_2, 0.1f, 0, 0);
		m_bAnimEnd[1] = true;
		m_bAnimEnd[2] = false;
	}
	else if (false == m_bAnimEnd[2] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_2))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_3, 0.1f, 0, 0);
		m_bAnimEnd[2] = true;
	}


	if (true == SkillCancel_Term())
		m_bIsSkillCancel = true;
	if (true == SkillEnd_Term())
		m_bIsSkillEnd = true;
}

void CSkill_GN_FocusShot::UnCtrl_Tick(_float fTimeDelta)
{
	if (false == m_bAnimEnd[0] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_1))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_2, 0.1f, 0, 0);
		m_bAnimEnd[0] = true;
		m_bAnimEnd[1] = false;
	}
	else if (false == m_bAnimEnd[1] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_2))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_2, 0.1f, 0, 0);
		m_bAnimEnd[1] = true;
		m_bAnimEnd[2] = false;
	}
	else if (false == m_bAnimEnd[2] && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_2))
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFocusShot_3, 0.1f, 0, 0);
		m_bAnimEnd[2] = true;
	}
}

HRESULT CSkill_GN_FocusShot::Render_Debug()
{
	return S_OK;
}

void CSkill_GN_FocusShot::Exit()
{
	m_bIsSkillCancel = false;
	m_bIsSkillEnd = false;
}

_bool CSkill_GN_FocusShot::SkillCancel_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Is_Dash())
		return true;

	return false;
}

_bool CSkill_GN_FocusShot::SkillEnd_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFocusShot_3))
		return true;

	return false;
}

CSkill_GN_FocusShot* CSkill_GN_FocusShot::Create(CGameObject* pOwner)
{
	CSkill_GN_FocusShot* pInstance = new CSkill_GN_FocusShot(pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CSkill_GN_FocusShot");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_GN_FocusShot::Free()
{
	__super::Free();
}
