#include "stdafx.h"
#include "UI_Inventory.h"
#include "GameInstance.h"

CUI_Inventory::CUI_Inventory(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Inventory::CUI_Inventory(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Inventory::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Inventory::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_Inventory");

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_Inventory::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Inventory::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_Inventory::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();
	return S_OK;
}

HRESULT CUI_Inventory::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	/*if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_Item_Upgrade_Rank"),
		TEXT("Com_Texturem"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CUI_Inventory::Bind_ShaderResources()
{
	__super::Bind_ShaderResources();
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

CUI_Inventory* CUI_Inventory::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Inventory* pInstance = new CUI_Inventory(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Inventory::Clone(void* pArg)
{
	CUI_Inventory* pInstance = new CUI_Inventory(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Inventory");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Inventory::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
