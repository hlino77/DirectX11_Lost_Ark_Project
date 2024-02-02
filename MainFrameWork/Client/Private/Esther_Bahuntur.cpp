#include "stdafx.h"
#include "..\Public\Esther_Bahuntur.h"
#include "GameInstance.h"
#include "Esther_Bahuntur_Cut.h"
#include "Esther_Bahuntur_Skill.h"

CEsther_Bahuntur::CEsther_Bahuntur(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther(pDevice, pContext)
{
}

CEsther_Bahuntur::CEsther_Bahuntur(const CEsther_Bahuntur& rhs)
	: CEsther(rhs)
{
}

HRESULT CEsther_Bahuntur::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Bahuntur::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Bahuntur");
	m_iEstherType = (_uint)ESTHERTYPE::BT;

	CEsther_Skill::ESTHERSKILLDESC SkillDesc;
	SkillDesc.pLeaderPlayer = m_pLeaderPlayer;
	SkillDesc.pOwnerEsther = this;
	m_pEsther_Skill = static_cast<CEsther_Bahuntur_Skill*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Bahuntur_Skill"), &SkillDesc));

	CEsther_Cut::ESTHERCUTDESC CutDesc;
	CutDesc.pLeaderPlayer = m_pLeaderPlayer;
	CutDesc.pOwnerEsther = this;
	m_pEsther_Cut = static_cast<CEsther_Bahuntur_Cut*>(m_pGameInstance->Clone_GameObject(TEXT("Prototype_GameObject_Esther_Bahuntur_Cut"), &CutDesc));

	return S_OK;
}

void CEsther_Bahuntur::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEsther_Bahuntur::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEsther_Bahuntur::Render()
{
	return S_OK;
}

void CEsther_Bahuntur::Leader_Active_Esther()
{
	__super::Leader_Active_Esther();
}

HRESULT CEsther_Bahuntur::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

CEsther_Bahuntur* CEsther_Bahuntur::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Bahuntur* pInstance = new CEsther_Bahuntur(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Bahuntur");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Bahuntur::Clone(void* pArg)
{
	CEsther_Bahuntur* pInstance = new CEsther_Bahuntur(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Bahuntur");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Bahuntur::Free()
{
	__super::Free();
}
