#include "stdafx.h"
#include "..\Public\Player_Controller_GN.h"
#include "GameInstance.h"

CPlayer_Controller_GN::CPlayer_Controller_GN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CPlayer_Controller(pDevice, pContext)
{
}

CPlayer_Controller_GN::CPlayer_Controller_GN(const CPlayer_Controller_GN& rhs)
	: CPlayer_Controller(rhs)
{
}

HRESULT CPlayer_Controller_GN::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	return S_OK;
}

void CPlayer_Controller_GN::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CPlayer_Controller_GN::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

void CPlayer_Controller_GN::DebugRender()
{
}

_bool CPlayer_Controller_GN::Is_Idle()
{
	if (false == __super::Is_Idle())
		return false;

	if (0 != Is_GN_Identity())
		return false;

	return true;
}

_uint CPlayer_Controller_GN::Is_GN_Identity()
{
	if (KEY_TAP(KEY::Z))
	{
		return 1;
	}
	else if (KEY_TAP(KEY::X))
	{
		return 2;
	}

	return 0;
}

HRESULT CPlayer_Controller_GN::Bind_HandSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pHandSkills[eKey] = pSkill;

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_ShotSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pShotSkills[eKey] = pSkill;

	return S_OK;
}

HRESULT CPlayer_Controller_GN::Bind_LongSkill(SKILL_KEY eKey, CPlayer_Skill* pSkill)
{
	if (nullptr == pSkill)
		return E_FAIL;

	if (SKILL_KEY::SPACE == eKey)
		return S_OK;

	m_pLongSkills[eKey] = pSkill;

	return S_OK;
}

void CPlayer_Controller_GN::Input(const _float& fTimeDelta)
{
}

void CPlayer_Controller_GN::Attack()
{
}

void CPlayer_Controller_GN::Hit()
{
}

void CPlayer_Controller_GN::GN_Identity(GN_IDENTITY eIndex)
{
	m_ePreIdentity = m_eIdentity;
	m_eIdentity = eIndex;
	Change_Skill_Iden(eIndex);
}

void CPlayer_Controller_GN::Change_Skill_Iden(GN_IDENTITY eIndex)
{
	if (GN_IDENTITY::HAND == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pHandSkills[i];
		}
	}
	else if (GN_IDENTITY::LONG == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pLongSkills[i];
		}
	}
	else if (GN_IDENTITY::SHOT == eIndex)
	{
		for (size_t i = 0; i < SKILL_KEY::_END; i++)
		{
			if (i == SKILL_KEY::SPACE) continue;

			m_pSkills[i] = m_pShotSkills[i];
		}
	}
}

CPlayer_Controller_GN* CPlayer_Controller_GN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CPlayer_Controller_GN::Clone(CGameObject* pGameObject, void* pArg)
{
	CPlayer_Controller_GN* pInstance = new CPlayer_Controller_GN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer_Controller_GN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer_Controller_GN::Free()
{
	__super::Free();
}