#include "..\Public\Player_Skill.h"

CPlayer_Skill::CPlayer_Skill(CGameObject* pOwner)
	: m_pOwner(pOwner)
{
}

HRESULT CPlayer_Skill::Initialize()
{
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
}

void CPlayer_Skill::Free()
{
	__super::Free();
}

