#include "stdafx.h"
#include "..\Public\Esther_Silian.h"
#include "GameInstance.h"
#include "Esther_Silian_Cut.h"
#include "Esther_Silian_Skill.h"

CEsther_Silian::CEsther_Silian(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CEsther(pDevice, pContext)
{
}

CEsther_Silian::CEsther_Silian(const CEsther_Silian& rhs)
	: CEsther(rhs)
{
}

HRESULT CEsther_Silian::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CEsther_Silian::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_strObjectTag = TEXT("Esther_Silian");

	m_pEsther_Skill = CEsther_Silian_Skill::Create(m_pDevice, m_pContext, m_pLeaderPlayer, nullptr);
	m_pEsther_Skill->Set_OwnerEsther(this);

	/*m_pEsther_Cut = CEsther_Silian_Cut::Create(m_pDevice, m_pContext, m_pLeaderPlayer, nullptr);
	m_pEsther_Cut->Set_OwnerEsther(this);*/

	m_iEstherType = (_uint)ESTHERTYPE::SA;

	return S_OK;
}

void CEsther_Silian::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CEsther_Silian::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CEsther_Silian::Render()
{
	return S_OK;
}

void CEsther_Silian::Leader_Active_Esther()
{
	__super::Leader_Active_Esther();
}

HRESULT CEsther_Silian::Ready_Components()
{
	__super::Ready_Components();

	return S_OK;
}

CEsther_Silian* CEsther_Silian::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Silian* pInstance = new CEsther_Silian(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CEsther_Silian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Silian::Clone(void* pArg)
{
	CEsther_Silian* pInstance = new CEsther_Silian(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEsther_Silian");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Silian::Free()
{
	__super::Free();
}
