#include "..\Public\Skill_GN_FreeShooter.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Player_Controller_GN.h"
#include "Model.h"

CSkill_GN_FreeShooter::CSkill_GN_FreeShooter(CGameObject* pOwner)
	: CPlayer_Skill(pOwner)
{
}

HRESULT CSkill_GN_FreeShooter::Initialize()
{
	/* ¼¦°Ç½ºÅ³ */
	m_eAttackType = { SKILL_ATTACKTYPE::NORMAL };
	m_eCtrlType = { SKILL_CTRLTYPE::NORMAL };
	m_IsSuperArmor = true;

	m_fSkillDamage = { 0.f };
	m_fSkillCoolTime = 6.f;

	m_iFreeShoter = static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Initailize_FindAnimation(L"sk_thefreeshooter", 1.2f);
	if (m_iFreeShoter == -1)
		return E_FAIL;

	return S_OK;
}

void CSkill_GN_FreeShooter::Enter()
{
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Reserve_Animation(m_iFreeShoter, 0.1f, 0, 0);

	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_StopMessage();
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_LerpLookMessage(static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_TargetPos());
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Get_SkillMessage(CPlayer_Controller_GN::GN_IDENTITY::SHOT, m_eBindCtrlKey);

	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, true);
}

void CSkill_GN_FreeShooter::Tick(_float fTimeDelta)
{
	if (110 <= static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Get_Anim_Frame(m_iFreeShoter))
		static_cast<CPlayer_Gunslinger*>(m_pOwner)->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);

	if (true == SkillCancel_Term())
		m_bIsSkillCancel = true;
	if (true == SkillEnd_Term())
		m_bIsSkillEnd = true;
}

void CSkill_GN_FreeShooter::UnCtrl_Tick(_float fTimeDelta)
{
}

HRESULT CSkill_GN_FreeShooter::Render_Debug()
{
	return S_OK;
}

void CSkill_GN_FreeShooter::Exit()
{
	static_cast<CPlayer_Gunslinger*>(m_pOwner)->Set_Several_Weapon_RenderState(CPartObject::PARTS::WEAPON_5, false);
	m_bIsSkillCancel = false;
	m_bIsSkillEnd = false;
}

_bool CSkill_GN_FreeShooter::SkillCancel_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Is_Dash() ||
		0 != static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_GN_Controller()->Is_GN_Identity())
			return true;

	return false;
}

_bool CSkill_GN_FreeShooter::SkillEnd_Term()
{
	if (true == static_cast<CPlayer_Gunslinger*>(m_pOwner)->Get_ModelCom()->Is_AnimationEnd(m_iFreeShoter))
		return true;
	
	return false;
}

CSkill_GN_FreeShooter* CSkill_GN_FreeShooter::Create(CGameObject* pOwner)
{
	CSkill_GN_FreeShooter* pInstance = new CSkill_GN_FreeShooter(pOwner);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CSkill_GN_FreeShooter");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CSkill_GN_FreeShooter::Free()
{
	__super::Free();
}
