#include "stdafx.h"
#include "UI_DeadWnd.h"
#include "GameInstance.h"
#include "TextBox.h"

CUI_DeadWnd::CUI_DeadWnd(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}
CUI_DeadWnd::CUI_DeadWnd(const CUI& rhs)
	: CUI(rhs)
{
}
HRESULT CUI_DeadWnd::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_DeadWnd::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_DeadWnd");

	m_fSizeX = 460.f;
	m_fSizeY = 240.f;
	m_fX = g_iWinSizeX - m_fSizeX * 0.6f;
	m_fY = m_fSizeY * 2.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_DeadWnd::Tick(_float fTimeDelta)
{
}

void CUI_DeadWnd::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_DeadWnd::Render()
{
	return S_OK;
}

HRESULT CUI_DeadWnd::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;

	//m_pTextureCom
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_DeadSceneUI"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_DeadScene_DeadSceneUI"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_AnimEffect"), (CComponent**)&m_pTransform_AnimEffect)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_WatchButton"), (CComponent**)&m_pTransform_WatchingButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Resurrect"), (CComponent**)&m_pTransform_ResurrectButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_NextButton"), (CComponent**)&m_pTransform_NextButton)))
		return E_FAIL;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_ReturnButton"), (CComponent**)&m_pTransform_ReturnButton)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_AnimFrame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_AnimEffect->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_AnimEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fAnimFrame);
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_ResurrectButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_ResurrectButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_WathchingButton()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_WatchingButton->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_Button->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_NextButton()
{
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_BackButton()
{
	return S_OK;
}

HRESULT CUI_DeadWnd::Bind_ShaderResources_ReturnkButton()
{
	return S_OK;
}

CUI_DeadWnd* CUI_DeadWnd::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_DeadWnd* pInstance = new CUI_DeadWnd(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_DeadWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_DeadWnd::Clone(void* pArg)
{
	CUI_DeadWnd* pInstance = new CUI_DeadWnd(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_DeadWnd");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_DeadWnd::Free()
{
	__super::Free();

	Safe_Release(m_pTexture_AnimEffect);
	Safe_Release(m_pTexture_Button);

	Safe_Release(m_pTransform_AnimEffect);
	Safe_Release(m_pTransform_WatchingButton);
	Safe_Release(m_pTransform_ResurrectButton);
	Safe_Release(m_pTransform_NextButton);
	Safe_Release(m_pTransform_ReturnButton);

	if (nullptr != m_pTextBox)
		m_pTextBox->Set_Dead(true);
}
