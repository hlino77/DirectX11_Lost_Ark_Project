#include "..\Public\Component.h"
#include "GameInstance.h"
#include "Transform.h"

CComponent::CComponent(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);

	m_pGameInstance = GET_INSTANCE(CGameInstance);
}

CComponent::CComponent(const CComponent & rhs)
	: m_pDevice(rhs.m_pDevice)
	, m_pContext(rhs.m_pContext)
	//, m_pOwner(rhs.m_pOwner)
	, m_pGameInstance(rhs.m_pGameInstance)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	//Safe_AddRef(m_pOwner);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CComponent::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CComponent::Initialize(void * pArg)
{
	return S_OK;
}

CTransform* CComponent::Get_TransformCom() const
{
	return m_pOwner->Get_TransformCom();
}


void CComponent::Free()
{
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	RELEASE_INSTANCE(CGameInstance);
}
