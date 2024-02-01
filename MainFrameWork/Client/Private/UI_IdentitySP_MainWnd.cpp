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

	m_strUITag = TEXT("UI_IdentitySp_MainWnd");

	m_fX = g_iWinSizeX * 0.5f;
	m_fY = 850.f;
	m_fSizeX = 189.f;
	m_fSizeY = 102.f;

	//m_pTransformCom
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_MainWnd
	m_PTransform_MainWnd->Set_Scale(Vec3(155.f, 155.f, 0.f));
	m_PTransform_MainWnd->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 20.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_BlueLight
	m_PTransform_BlueLight->Set_Scale(Vec3(869.f * 0.5f, 233.f * 0.5f, 0.f));
	m_PTransform_BlueLight->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 45.f) - g_iWinSizeX * 0.5f, -(m_fY - 60.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_Ink
	m_PTransform_Ink->Set_Scale(Vec3(189.f * 0.9f, 121.f * 0.9f, 0.f));//(Vec3(95.f, 75.f, 0.f));
	m_PTransform_Ink->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 40.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_Brush
	m_PTransform_Brush->Set_Scale(Vec3(73.f, 178.f, 0.f));
	m_PTransform_Brush->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX + 10.f) - g_iWinSizeX * 0.5f, -(m_fY -30.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_BubbleFrmaes
	m_PTransform_BubbleFrmaes->Set_Scale(Vec3(139.f, 56.f, 0.f));
	m_PTransform_BubbleFrmaes->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY + 25.f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_PTransform_BrushEffect
	m_PTransform_BrushEffect->Set_Scale(Vec3(252.f * 0.6f, 252.f * 0.6f, 0.f));
	m_PTransform_BrushEffect->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -(m_fY - 60.f) + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentitySP_MainWnd::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (59.f > m_fFrame)
		m_fFrame += 20.f * fTimeDelta;
	if (59.f <= m_fFrame)
		m_fFrame = 0.f;
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

	if (FAILED(Bind_ShaderResources_BrushEffect()))
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Blue_Light"),
		TEXT("Com_Texture_BlueLight"), (CComponent**)&m_pTexture_BlueLight)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Ink"),
		TEXT("Com_Texture_Ink"), (CComponent**)&m_pTexture_Ink)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Brush"),
		TEXT("Com_Texture_Brush"), (CComponent**)&m_pTexture_Brush)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Bubble_Frames"),
		TEXT("Com_Texture_BubbleFrmaes"), (CComponent**)&m_pTexture_BubbleFrmaes)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Identity_Yinyangshi_Brush_Effect"),
		TEXT("Com_Texture_BrushEffect"), (CComponent**)&m_pTexture_BrushEffect)))
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
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_BrushEffect"), (CComponent**)&m_PTransform_BrushEffect)))
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

HRESULT CUI_IdentitySP_MainWnd::Bind_ShaderResources_BrushEffect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_PTransform_BrushEffect->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_BrushEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame);
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

	Safe_Release(m_pTexture_MainWnd);
	Safe_Release(m_pTexture_BlueLight);
	Safe_Release(m_pTexture_Ink);
	Safe_Release(m_pTexture_Brush);
	Safe_Release(m_pTexture_BubbleFrmaes);
	Safe_Release(m_pTexture_BrushEffect);

	Safe_Release(m_PTransform_MainWnd);
	Safe_Release(m_PTransform_BlueLight);
	Safe_Release(m_PTransform_Ink);
	Safe_Release(m_PTransform_Brush);
	Safe_Release(m_PTransform_BubbleFrmaes);
	Safe_Release(m_PTransform_BrushEffect);
}
