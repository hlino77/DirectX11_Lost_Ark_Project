#include "stdafx.h"
#include "UI_IdentitySP_MainWnd.h"
#include "GameInstance.h"

CUI_IdentitySP_MainWnd::CUI_IdentitySP_MainWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentitySP_MainWnd::CUI_IdentitySP_MainWnd(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentitySP_MainWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY - 200.f;
	m_fSizeX = 189.f;
	m_fSizeY = 102.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentitySP_MainWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentitySP_MainWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentitySP_MainWnd::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_MainWnd()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BlueLight()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Ink()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Brush()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_BubbleFrames()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_BackGround"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_MainWnd"),
		TEXT("Com_Texture_MainWnd"), (CComponent**)&m_pTexture_MainWnd)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Ink"),
		TEXT("Com_Texture_BlueLight"), (CComponent**)&m_pTexture_BlueLight)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Masked_GaugeCut"),
		TEXT("Com_Texture_Ink"), (CComponent**)&m_pTexture_Ink)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Brush"),
		TEXT("Com_Texture_Brush"), (CComponent**)&m_pTexture_Brush)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Frames"),
		TEXT("Com_Texture_BubbleFrmaes"), (CComponent**)&m_pTexture_BubbleFrmaes)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_MainWnd"), (CComponent**)&m_PTransform_MainWnd)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BlueLight"), (CComponent**)&m_PTransform_BlueLight)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Ink"), (CComponent**)&m_PTransform_Ink)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Brush"), (CComponent**)&m_PTransform_Brush)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BubbleFrmaes"), (CComponent**)&m_PTransform_BubbleFrmaes)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_MainWnd()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_MainWnd->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_MainWnd->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_BlueLight()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_BlueLight->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BlueLight->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_Ink()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_Ink->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_Ink->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_Brush()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_Brush->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_Brush->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_BubbleFrames()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_BubbleFrmaes->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BubbleFrmaes->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

CUI_IdentitySP_MainWnd* CUI_IdentitySP_MainWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentitySP_MainWnd* pInstance = new CUI_IdentitySP_MainWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentitySP_MainWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentitySP_MainWnd::Clone(void* pArg)
{
	CUI_IdentitySP_MainWnd* pInstance = new CUI_IdentitySP_MainWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentitySP_MainWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentitySP_MainWnd::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTexture_MainWnd);
	Safe_Release(m_pTexture_BlueLight);
	Safe_Release(m_pTexture_Ink);
	Safe_Release(m_pTexture_Brush);
	Safe_Release(m_pTexture_BubbleFrmaes);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_PTransform_MainWnd);
	Safe_Release(m_PTransform_BlueLight);
	Safe_Release(m_PTransform_Ink);
	Safe_Release(m_PTransform_Brush);
	Safe_Release(m_PTransform_BubbleFrmaes);

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
