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

	m_pEsther_Skill = CEsther_Bahuntur_Skill::Create(m_pDevice, m_pContext, m_pLeaderPlayer, nullptr);
	m_pEsther_Skill->Set_OwnerEsther(this);

	m_iEstherType = (_uint)ESTHERTYPE::BT;

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
