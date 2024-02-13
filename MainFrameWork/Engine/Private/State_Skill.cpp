#include "..\Public\State_Skill.h"
#include "StateMachine.h"
#include "Player_Controller.h"
#include "Player_Skill.h"

CState_Skill::CState_Skill(const wstring& strStateName, CStateMachine* pMachine, CPlayer_Controller* pController)
	: CState(strStateName, pMachine, pController)
{
}

HRESULT CState_Skill::Initialize()
{
	return S_OK;
}

void CState_Skill::Bind_SkillKey(CPlayer_Controller::SKILL_KEY eKey)
{
	switch (eKey)
	{
	case Engine::CPlayer_Controller::Q:
		m_eSkillSelectKey = CPlayer_Controller::Q;
		m_eSkillBindKey = KEY::Q;
		break;
	case Engine::CPlayer_Controller::W:
		m_eSkillSelectKey = CPlayer_Controller::W;
		m_eSkillBindKey = KEY::W;
		break;
	case Engine::CPlayer_Controller::E:
		m_eSkillSelectKey = CPlayer_Controller::E;
		m_eSkillBindKey = KEY::E;
		break;
	case Engine::CPlayer_Controller::R:
		m_eSkillSelectKey = CPlayer_Controller::R;
		m_eSkillBindKey = KEY::R;
		break;
	case Engine::CPlayer_Controller::A:
		m_eSkillSelectKey = CPlayer_Controller::A;
		m_eSkillBindKey = KEY::A;
		break;
	case Engine::CPlayer_Controller::S:
		m_eSkillSelectKey = CPlayer_Controller::S;
		m_eSkillBindKey = KEY::S;
		break;
	case Engine::CPlayer_Controller::D:
		m_eSkillSelectKey = CPlayer_Controller::D;
		m_eSkillBindKey = KEY::D;
		break;
	case Engine::CPlayer_Controller::F:
		m_eSkillSelectKey = CPlayer_Controller::F;
		m_eSkillBindKey = KEY::F;
		break;
	}
}

void CState_Skill::Free()
{
	__super::Free();

}
