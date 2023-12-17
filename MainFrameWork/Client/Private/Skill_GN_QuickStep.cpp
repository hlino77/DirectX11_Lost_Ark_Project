#include "..\Public\Skill_GN_QuickStep.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CSkill_GN_QuickStep::CSkill_GN_QuickStep(CGameObject* pOwner)
	: CPlayer_Skill(pOwner)
{
}

HRESULT CSkill_GN_QuickStep::Initialize()
{
	/* 핸드건스킬 */
	m_eAttackType = { SKILL_ATTACKTYPE::NORMAL };
	m_eCtrlType = { SKILL_CTRLTYPE::COMBO };
	m_IsSuperArmor = true;

	m_fSkillDamage = { 0.f };
	m_fSkillCoolTime = 3.f;

	m_iQuickStep_1 = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_hallucination_01", 1.2f);
	if (m_iQuickStep_1 == -1)
		return E_FAIL;

	m_iQuickStep_2 = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_hallucination_02", 1.2f);
	if (m_iQuickStep_2 == -1)
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_QuickStep::Enter()
{
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iQuickStep_1, 0.1f, 0, 0);

	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_StopMessage();
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_LookMessage(static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_TargetPos());
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::HAND, m_eBindCtrlKey);
}

void CSkill_GN_QuickStep::Tick(_float fTimeDelta)
{
	if (false == m_bCombo && true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Is_Tap(m_eBindKey) 
		&& 8 >= static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_1))
	{
		m_bCombo = true;
	}
	Vec3 vCombo = Vec3(m_bCombo, 0.f, 0.f);
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Set_TargetPos(vCombo);

	if (10 == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_1) &&
		true == m_bCombo && false == m_bCombo2)
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iQuickStep_2, 0.1f, 0, 0);
		m_bCombo2 = true;
	}	

	if (true == SkillCancel_Term())
		m_bIsSkillCancel = true;
	if (true == SkillEnd_Term())
		m_bIsSkillEnd = true;
}

void CSkill_GN_QuickStep::UnCtrl_Tick(_float fTimeDelta)
{
	m_bCombo = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_TargetPos().x;

	if (10 == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(m_iQuickStep_1) &&
		true == m_bCombo && false == m_bCombo2)
	{
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iQuickStep_2, 0.1f, 0, 0);
		m_bCombo2 = true;
	}
}

HRESULT CSkill_GN_QuickStep::Render_Debug()
{
	return S_OK;
}

void CSkill_GN_QuickStep::Exit()
{
	m_bIsSkillCancel = false;
	m_bIsSkillEnd = false;
	m_bCombo = false;
	m_bCombo2 = false;
}

_bool CSkill_GN_QuickStep::SkillCancel_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iQuickStep_1))
		return true;

	return false;
}

_bool CSkill_GN_QuickStep::SkillEnd_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iQuickStep_2))
		return true;

	return false;
}

CSkill_GN_QuickStep* CSkill_GN_QuickStep::Create(CGameObject* pOwner)
{
	CSkill_GN_QuickStep* pInstance = new CSkill_GN_QuickStep(pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CSkill_GN_QuickStep");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_GN_QuickStep::Free()
{
	__super::Free();
}
