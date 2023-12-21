#include "..\Public\Player_Skill.h"
#include "State_Skill.h"
#include "ColliderSphere.h"

CPlayer_Skill::CPlayer_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType)
	: CGameObject(pDevice, pContext, strObjectTag, iObjType)
{
}

HRESULT CPlayer_Skill::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		PLAYERSKILL_DESC* pSkillDesc = (PLAYERSKILL_DESC*)pArg;
		m_strSkill_StartName = pSkillDesc->strSkill_StartName;
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

void CPlayer_Skill::Check_ColliderState()
{
	for (auto& pSkillState : m_State_Skills)
	{
		if (true == static_cast<CState_Skill*>(pSkillState)->Is_SkillAttack())
		{
			for (auto& Collider : m_Coliders)
			{
				Collider.second->SetActive(true);
				Collider.second->Update_Collider();
			}
		}
		else
		{
			for (auto& Collider : m_Coliders)
			{
				Collider.second->SetActive(false);
			}
		}
	}
}

void CPlayer_Skill::Free()
{
	__super::Free();

	Safe_Release(m_pSkillTextureCom);
}

