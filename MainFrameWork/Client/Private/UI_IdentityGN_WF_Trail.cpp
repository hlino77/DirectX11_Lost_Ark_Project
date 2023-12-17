#include "stdafx.h"
#include "UI_IdentityGN_WF_Trail.h"
#include "GameInstance.h"

CUI_IdentityGN_WF_Trail::CUI_IdentityGN_WF_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}
CUI_IdentityGN_WF_Trail::CUI_IdentityGN_WF_Trail(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_IdentityGN_WF_Trail::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_IdentityGN_WF_Trail::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_WeaponFrame_Trail");

	m_fSizeX = 400.f;
	m_fSizeY = 268.f;
	m_fX = 12.f;
	m_fY = 543.f;
	m_fAlpha = 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	return S_OK;
}

void CUI_IdentityGN_WF_Trail::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentityGN_WF_Trail::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_WF_Trail::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentityGN_WF_Trail::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_ShineTrail"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_WF_Trail::Bind_ShaderResources()
{
	if (__super::Bind_ShaderResources())
		return E_FAIL;


	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
}

CUI_IdentityGN_WF_Trail* CUI_IdentityGN_WF_Trail::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_WF_Trail* pInstance = new CUI_IdentityGN_WF_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_WF_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_WF_Trail::Clone(void* pArg)
{
	CUI_IdentityGN_WF_Trail* pInstance = new CUI_IdentityGN_WF_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_WF_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_WF_Trail::Free()
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
