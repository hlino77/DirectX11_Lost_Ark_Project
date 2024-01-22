#include "stdafx.h"
#include "UI_LoadingWnd.h"
#include "GameInstance.h"

CUI_LoadingWnd::CUI_LoadingWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_LoadingWnd::CUI_LoadingWnd(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_LoadingWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_LoadingWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Loading_Wnd");

	m_fX	=	g_iWinSizeX * 0.5f;
	m_fY	=	g_iWinSizeY * 0.5f;
	m_fSizeX	= g_iWinSizeX;
	m_fSizeY	=	g_iWinSizeY;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_LoadingWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_LoadingWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingWnd::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcesImg()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResourcesFrame()))
		return E_FAIL;

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_LoadingWnd::Set_TextureIndex(_uint iLevelIndex)
{
	if ((_uint)LEVEL_BERN == iLevelIndex)
		m_iTextureIndex = 0;
	else if (((_uint)LEVEL_CHAOS_1 == iLevelIndex) || ((_uint)LEVEL_CHAOS_2 == iLevelIndex) ||
		((_uint)LEVEL_CHAOS_3 == iLevelIndex))
		m_iTextureIndex = 0;
	else if (((_uint)LEVEL_VALTANENTERANCE == iLevelIndex) || (_uint)LEVEL_VALTANMAIN == iLevelIndex)
		m_iTextureIndex = 1;
}

HRESULT CUI_LoadingWnd::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingBackGround"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingImgs"),
		TEXT("Com_TextureImg"), (CComponent**)&m_pTextureComImg)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Frame"),
		TEXT("Com_TextureFrame"), (CComponent**)&m_pTextureCom_Frame)))
		return E_FAIL;
	
	return S_OK;
}

HRESULT CUI_LoadingWnd::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
}

HRESULT CUI_LoadingWnd::Bind_ShaderResourcesImg()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureComImg->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
}

HRESULT CUI_LoadingWnd::Bind_ShaderResourcesFrame()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
}

CUI_LoadingWnd* CUI_LoadingWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_LoadingWnd* pInstance = new CUI_LoadingWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_LoadingWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_LoadingWnd::Clone(void* pArg)
{
	CUI_LoadingWnd* pInstance = new CUI_LoadingWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_LoadingWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_LoadingWnd::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureComImg);
	Safe_Release(m_pTextureCom_Frame);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
