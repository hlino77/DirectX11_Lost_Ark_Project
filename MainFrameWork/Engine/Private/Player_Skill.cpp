#include "..\Public\Player_Skill.h"
#include "State_Skill.h"

CPlayer_Skill::CPlayer_Skill(CGameObject* pOwner)
	: m_pOwner(pOwner)
{
}

HRESULT CPlayer_Skill::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		PLAYERSKILL_DESC* pSkillDesc = (PLAYERSKILL_DESC*)pArg;
		m_strSkill_StartName = pSkillDesc->strSkill_StartName;
		m_eAttackType = pSkillDesc->eAttackType;
		m_eCtrlType = pSkillDesc->eCtrlType;

		m_IsSuperArmor = pSkillDesc->IsSuperArmor;

		m_fSkillDamage = pSkillDesc->fSkillDamage;
		m_fSkillCoolTime = pSkillDesc->fSkillCoolTime;

		m_State_Skills = pSkillDesc->State_Skills;
	}

	return S_OK;
}

void CPlayer_Skill::Set_BindKey(CPlayer_Controller::SKILL_KEY eKey)
{
	switch (eKey)
	{
	case Engine::CPlayer_Controller::SPACE:
		m_eBindKey = KEY::SPACE;
		break;
	case Engine::CPlayer_Controller::Q:
		m_eBindKey = KEY::Q;
		break;
	case Engine::CPlayer_Controller::W:
		m_eBindKey = KEY::W;
		break;
	case Engine::CPlayer_Controller::E:
		m_eBindKey = KEY::E;
		break;
	case Engine::CPlayer_Controller::R:
		m_eBindKey = KEY::R;
		break;
	case Engine::CPlayer_Controller::A:
		m_eBindKey = KEY::A;
		break;
	case Engine::CPlayer_Controller::S:
		m_eBindKey = KEY::S;
		break;
	case Engine::CPlayer_Controller::D:
		m_eBindKey = KEY::D;
		break;
	case Engine::CPlayer_Controller::F:
		m_eBindKey = KEY::F;
		break;
	}
	m_eBindCtrlKey = eKey;

	for (auto* Skill : m_State_Skills)
	{
		static_cast<CState_Skill*>(Skill)->Bind_SkillKey(m_eBindCtrlKey);
	}
}

CPlayer_Skill* CPlayer_Skill::Create(CGameObject* pOwner, void* pArg)
{
	CPlayer_Skill* pInstance = new CPlayer_Skill(pOwner);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Created : CSkill_GN_QuickStep");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Skill::Free()
{
	__super::Free();
}

