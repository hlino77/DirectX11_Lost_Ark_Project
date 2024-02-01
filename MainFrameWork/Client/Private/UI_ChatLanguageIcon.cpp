#include "stdafx.h"
#include "UI_ChatLanguageIcon.h"
#include "GameInstance.h"
#include "Chat_Manager.h"

CUI_ChatLanguageIcon::CUI_ChatLanguageIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChatLanguageIcon::CUI_ChatLanguageIcon(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChatLanguageIcon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChatLanguageIcon::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("Chat_LanguageIcon");

	m_fSizeX = 45.f;
	m_fSizeY = 45.f;
	m_fX = 412.f;
	m_fY = 711.f;
	m_fAlpha = 0.8f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_ChatLanguageIcon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ChatLanguageIcon::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_ChatLanguageIcon::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_ChatLanguageIcon::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_State"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_ChatLanguageIcon::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_iLanguageState = CChat_Manager::GetInstance()->Get_Language();
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iLanguageState);

	return S_OK;
}

CUI_ChatLanguageIcon* CUI_ChatLanguageIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChatLanguageIcon* pInstance = new CUI_ChatLanguageIcon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChatLanguageIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChatLanguageIcon::Clone(void* pArg)
{
	CUI_ChatLanguageIcon* pInstance = new CUI_ChatLanguageIcon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChatLanguageIcon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChatLanguageIcon::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
