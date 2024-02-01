#include "stdafx.h"
#include "UI_WRIdentity_Boomb.h"
#include "GameInstance.h"
#include "UI_WRIdentity_Body.h"
#include "ServerSessionManager.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"

CUI_WRIdentity_Boomb::CUI_WRIdentity_Boomb(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WRIdentity_Boomb::CUI_WRIdentity_Boomb(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WRIdentity_Boomb::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WRIdentity_Boomb::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		m_pBodyUI = static_cast<CUI*>(pArg);
	}

	m_strUITag = TEXT("WRIdentity_Boomb");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = 200.f;
	m_fY = 200.f;
	m_fAlpha = 1.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) +(m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_WRIdentity_Boomb::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (!m_bBommbOn)
	{
		if (CUI_WRIdentity_Body::WRIDENTITY_TRANSFORM_ON == static_cast<CUI_WRIdentity_Body*>(m_pBodyUI)->Get_Current_IdentityState())
			m_bBommbOn = true;
	}


	if (m_bBommbOn)
	{
		if (!m_bHidden)
		{
			m_fSizeX += 10.f * fTimeDelta;
			m_fSizeY += 10.f * fTimeDelta;
			m_fAlpha = 1.f;

			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
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
			}
			if (0 >= m_fAlpha)
			{
				m_fSizeX = m_vecSizeOrigin.x;
				m_fSizeY = m_vecSizeOrigin.y;
				m_bHidden = false;
				m_bBommbOn = false;
				m_fAlpha = 0.f;
			}
		}
	}
}

void CUI_WRIdentity_Boomb::LateTick(_float fTimeDelta)
{
	if(m_bBommbOn)
		__super::LateTick(fTimeDelta);
}

HRESULT CUI_WRIdentity_Boomb::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_WRIdentity_Boomb::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Spark_Effect"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WRIdentity_Boomb::Bind_ShaderResources()
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

CUI_WRIdentity_Boomb* CUI_WRIdentity_Boomb::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WRIdentity_Boomb* pInstance = new CUI_WRIdentity_Boomb(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WRIdentity_Boomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WRIdentity_Boomb::Clone(void* pArg)
{
	CUI_WRIdentity_Boomb* pInstance = new CUI_WRIdentity_Boomb(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WRIdentity_Boomb");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WRIdentity_Boomb::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
