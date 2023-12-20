#include "stdafx.h"
#include "UI_IdentityGN_Spark.h"
#include "GameInstance.h"
#include "UI_IdentityGN_MainFrame.h"

CUI_IdentityGN_Spark::CUI_IdentityGN_Spark(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_Spark::CUI_IdentityGN_Spark(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_Spark::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_Spark::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		m_pMainFrame = static_cast<CUI*>(pArg);
	}

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_Spark");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) +(m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentityGN_Spark::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if (!m_bChange)
		return;

	if (!m_bHidden)
	{
			if (!static_cast<CUI_IdentityGN_MainFrame*>(m_pMainFrame)->Get_IsTapKey())
				m_fX = 750.f;
			else if (static_cast<CUI_IdentityGN_MainFrame*>(m_pMainFrame)->Get_IsTapKey())
				m_fX = 850.f;
			m_fSizeX += 10.f * fTimeDelta;
			m_fSizeY += 10.f * fTimeDelta;
			m_fAlpha = 0.8f;

			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
			m_bHidden = true;
	}
	else if (m_bHidden)
	{
		if (0 < m_fAlpha)
		{
			m_fSizeX += 300.f * fTimeDelta;
			m_fSizeY += 300.f * fTimeDelta;
			m_fAlpha -= 1.5f * fTimeDelta;
			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
		}
		if (0 >= m_fAlpha)
		{
			m_fSizeX = m_vecSizeOrigin.x;
			m_fSizeY = m_vecSizeOrigin.y;
			m_bHidden = false;
			m_fAlpha = 0.f;
			m_bChange = false;
		}
	}
}

void CUI_IdentityGN_Spark::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_Spark::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentityGN_Spark::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponChange_Spark"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_Spark::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_IdentityGN_Spark* CUI_IdentityGN_Spark::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_Spark* pInstance = new CUI_IdentityGN_Spark(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_Spark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_Spark::Clone(void* pArg)
{
	CUI_IdentityGN_Spark* pInstance = new CUI_IdentityGN_Spark(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_Spark");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_Spark::Free()
{
	__super::Free();
	m_pMainFrame = nullptr;
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
