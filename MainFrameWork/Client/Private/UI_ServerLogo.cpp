#include "stdafx.h"
#include "UI_ServerLogo.h"
#include "GameInstance.h"

CUI_ServerLogo::CUI_ServerLogo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ServerLogo::CUI_ServerLogo(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ServerLogo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ServerLogo::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Server_Logo");

	m_fSizeX = 804;
	m_fSizeY = 435;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = (g_iWinSizeY * 0.5f) - 300.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_ServerLogo::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ServerLogo::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ServerLogo::Render()
{
	__super::Render();

	return S_OK;
}

HRESULT CUI_ServerLogo::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_LostArkLogo"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ServerLogo::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_ServerLogo* CUI_ServerLogo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ServerLogo* pInstance = new CUI_ServerLogo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ServerLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ServerLogo::Clone(void* pArg)
{
	CUI_ServerLogo* pInstance = new CUI_ServerLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ServerLogo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ServerLogo::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
