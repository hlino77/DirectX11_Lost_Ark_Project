#include "stdafx.h"
#include "UI_IdentityGN_WF_BG.h"
#include "GameInstance.h"

CUI_IdentityGN_WF_BG::CUI_IdentityGN_WF_BG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_WF_BG::CUI_IdentityGN_WF_BG(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_WF_BG::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_WF_BG::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_WeaponFrame_BackGround");

	m_fSizeX = 400.f;
	m_fSizeY = 268.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	return S_OK;
}

void CUI_IdentityGN_WF_BG::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentityGN_WF_BG::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_WF_BG::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentityGN_WF_BG::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_Back"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_WF_BG::Bind_ShaderResources()
{
	if(__super::Bind_ShaderResources())
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex);

	return S_OK;
}

CUI_IdentityGN_WF_BG* CUI_IdentityGN_WF_BG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_WF_BG* pInstance = new CUI_IdentityGN_WF_BG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_WF_BG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_WF_BG::Clone(void* pArg)
{
	CUI_IdentityGN_WF_BG* pInstance = new CUI_IdentityGN_WF_BG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_WF_BG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_WF_BG::Free()
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
