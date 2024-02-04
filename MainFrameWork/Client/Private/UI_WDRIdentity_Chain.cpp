#include "stdafx.h"
#include "UI_WDRIdentity_Chain.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Controller_WDR.h"
#include "Player_Destroyer.h"
#include "UI_WDRIdentity_Gauge.h"
#include "UI_WDRIdentity_Hammer.h"

CUI_WDRIdentity_Chain::CUI_WDRIdentity_Chain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WDRIdentity_Chain::CUI_WDRIdentity_Chain(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WDRIdentity_Chain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity_Chain::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityWDR_Chain");

	if (nullptr != pArg)
	{
		CHAIN_DESC* ChainDesc = static_cast<CHAIN_DESC*>(pArg);

		m_pGaugeUI = ChainDesc->pGaugeUI;
		m_pHammerUI = ChainDesc->pHammerUI;
	}

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_WDRIdentity_Chain::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_Chain_State(fTimeDelta);
}

void CUI_WDRIdentity_Chain::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WDRIdentity_Chain::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	if(!m_bCutting)
		m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame);
	else 
		m_pTexture_ChainCut->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame);

	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_WDRIdentity_Chain::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Chain"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Chain_Cut"),
		TEXT("Com_TextureCut"), (CComponent**)&m_pTexture_ChainCut)))
		return E_FAIL;

	return S_OK; 
}

HRESULT CUI_WDRIdentity_Chain::Bind_ShaderResources()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	return S_OK;
}

void CUI_WDRIdentity_Chain::Update_Chain_State(_float fTimeDelta)
{
	if (nullptr != m_pGaugeUI)
	{
		m_fCurrRatio = static_cast<CUI_WDRIdentity_Gauge*>(m_pGaugeUI)->Get_Identity_Ratio();
		m_bIdentityOn = static_cast<CUI_WDRIdentity_Gauge*>(m_pGaugeUI)->Get_IdentityOn();

		if ((!m_bIdentityOn)&&(CHAIN_NONE != m_iCurrChaniState))
		{
			if (!m_bCutting)
			{
				if (0.3f > m_fCurrRatio)
				{
					m_bRender = true;
					m_iCurrChaniState = CHAIN_THREE;
				}
				if ((0.3f <= m_fCurrRatio) && (0.6f > m_fCurrRatio)&&((_uint)CHAIN_TWO > m_iCurrChaniState))
				{
					m_bRender = true;
					m_bCutting = true;
					m_fFrame = 0.f;
					m_iCurrChaniState = CHAIN_CUT1;
				}

				if ((0.6f <= m_fCurrRatio) && (0.88f > m_fCurrRatio)&& ((_uint)CHAIN_ONE > m_iCurrChaniState))
				{
					m_bRender = true;
					m_bCutting = true;
					m_fFrame = 16.f;
					m_iCurrChaniState = CHAIN_CUT2;
				}

				if ((1.f <= m_fCurrRatio)&&(((_uint)CHAIN_NONE > m_iCurrChaniState)))
				{
					m_bRender = true;
					m_bCutting = true;
					m_fFrame = 33.f;
					m_iCurrChaniState = CHAIN_CUT3;
				}
				Update_Chain_Idle(fTimeDelta);
			}
			else
				Update_Chain_Cutting(fTimeDelta);
		}
		else
		{
			m_bRender = false;
			m_fFrame = 0.f;

			if (CUI_WDRIdentity_Hammer::HAMMER_TRANSFORM_OFF == static_cast<CUI_WDRIdentity_Hammer*>(m_pHammerUI)->Get_CurrHammerState())
			{
				m_bRender = true;
				m_iCurrChaniState = CHAIN_THREE;
			}
		}
	}
}

void CUI_WDRIdentity_Chain::Update_Chain_Idle(_float fTimeDelta)
{
	if (m_bCutting)
		return;

	m_fDelay += fTimeDelta;
	if (3.f <= m_fDelay)
	{
		if (CHAIN_THREE == m_iCurrChaniState)
		{
			if (4.f >= m_fFrame)
				m_fFrame += 10.f * fTimeDelta;
			if (4.f <= m_fFrame)
			{
				m_fDelay = 0.f;
				m_fFrame = 0.f;
			}
		}

		if (CHAIN_TWO == m_iCurrChaniState)
		{
			if (9.f >= m_fFrame)
				m_fFrame += 10.f * fTimeDelta;
			if (9.f < m_fFrame)
			{
				m_fDelay = 0.f;
				m_fFrame = 5.f;
			}
		}

		if (CHAIN_ONE == m_iCurrChaniState)
		{
			if (14.f >= m_fFrame)
				m_fFrame += 10.f * fTimeDelta;
			if (14.f < m_fFrame)
			{
				m_fDelay = 0.f;
				m_fFrame = 10.f;
			}
		}
		if(CHAIN_NONE == m_iCurrChaniState)
		{
			m_bRender = false;
		}
	}
}

void CUI_WDRIdentity_Chain::Update_Chain_Cutting(_float fTimeDelta)
{
	if (!m_bCutting)
		return;

	if (CHAIN_CUT1 == m_iCurrChaniState)
	{
		if (15.f >= m_fFrame)
			m_fFrame += 25.f * fTimeDelta;
		if (15.f < m_fFrame)
		{
			m_fFrame = 5.f;
			m_iCurrChaniState = CHAIN_TWO;
			m_bCutting = false;
		}
	}
	if (CHAIN_CUT2 == m_iCurrChaniState)
	{
		if (32.f >= m_fFrame)
			m_fFrame += 25.f * fTimeDelta;
		if (32.f < m_fFrame)
		{
			m_fFrame = 10.f;
			m_iCurrChaniState = CHAIN_ONE;
			m_bCutting = false;
		}
	}
	if (CHAIN_CUT3 == m_iCurrChaniState)
	{
		if (49.f >= m_fFrame)
			m_fFrame += 25.f * fTimeDelta;
		if (49.f <= m_fFrame)
		{
			m_fFrame = 0.f;
			m_iCurrChaniState = CHAIN_NONE;
			m_bCutting = false;
			m_bRender = false;
		}
	}

}

CUI_WDRIdentity_Chain* CUI_WDRIdentity_Chain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WDRIdentity_Chain* pInstance = new CUI_WDRIdentity_Chain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WDRIdentity_Chain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WDRIdentity_Chain::Clone(void* pArg)
{
	CUI_WDRIdentity_Chain* pInstance = new CUI_WDRIdentity_Chain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WDRIdentity_Chain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WDRIdentity_Chain::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTexture_ChainCut);
}
