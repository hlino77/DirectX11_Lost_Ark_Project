#include "stdafx.h"
#include "UI_IdentityMG_Gauge.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Controller_MG.h"
#include "Player_Bard.h"

CUI_IdentityMG_Gauge::CUI_IdentityMG_Gauge(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityMG_Gauge::CUI_IdentityMG_Gauge(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityMG_Gauge::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityMG_Gauge::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if (nullptr != pArg)
	{
		_uint* strIndex = static_cast<_uint*>(pArg);
		m_iGaugeIndex = *strIndex+1;
		m_strUITag = TEXT("IdentityMG_Gauge");
		m_strUITag += to_wstring(m_iGaugeIndex);
	}
	else
		m_strUITag = TEXT("IdentityMG_Gauge");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_Gauge->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_pTransform_Cut->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransform_Cut->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	
	m_pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != m_pPlayer)
		m_iIdentity_MaxGauge = static_cast<CPlayer_Bard*>(m_pPlayer)->
		Get_MG_Controller()->Get_IdenMaxGauge();

	return S_OK;
}

void CUI_IdentityMG_Gauge::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Setting_GaugeScale();
	Update_Identity_Gauge();
}

void CUI_IdentityMG_Gauge::LateTick(_float fTimeDelta)
{
	if (1.f <= m_fIdentity_Ratio)
		m_iTextureIndex = 1;
	else
		m_iTextureIndex = 0;

	if (0.f >= m_iIdentity_CurrGauge)
		m_fIdentity_Ratio = 0.f;

	Update_Identity_Cut();
	__super::LateTick(fTimeDelta);

}

HRESULT CUI_IdentityMG_Gauge::Render()
{
	//Empty
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_Empty->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	//Gauge
	if (FAILED(Bind_ShaderResources_Gauge()))
		return E_FAIL;
	m_pShaderCom->Begin(12);
	m_pVIBufferCom->Render();

	//Frame
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();
	
	//Cut
	if (FAILED(Bind_ShaderResources_Cut()))
		return E_FAIL;
	m_pTextureCom_Cut->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iGaugeCutTextureIndex);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentityMG_Gauge::Ready_Components()
{
	__super::Ready_Components();
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeEmpty"),
		TEXT("Com_Texture_Empty"), (CComponent**)&m_pTextureCom_Empty)))
		return E_FAIL;
	
	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Gauge"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeFrame"),
		TEXT("Com_Texture_Frame"), (CComponent**)&m_pTextureCom_Frame)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_GaugeCut"),
		TEXT("Com_Texture_Cut"), (CComponent**)&m_pTextureCom_Cut)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Gauge"), (CComponent**)&m_pTransform_Gauge)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Cut"), (CComponent**)&m_pTransform_Cut)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityMG_Gauge::Bind_ShaderResources()
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

HRESULT CUI_IdentityMG_Gauge::Bind_ShaderResources_Gauge()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Gauge->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fIdentity_Ratio, sizeof(_float))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex);

	return S_OK;
}

HRESULT CUI_IdentityMG_Gauge::Bind_ShaderResources_Cut()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Cut->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
}

void CUI_IdentityMG_Gauge::Update_Identity_Gauge()
{
	if (nullptr != m_pPlayer)
	{
		m_iIdentity_CurrGauge = static_cast<CPlayer_Bard*>(m_pPlayer)->
			Get_MG_Controller()->Get_IdenGage();

		switch (m_iGaugeIndex)
		{
		case 1:
			if (1.f > m_fIdentity_Ratio)
			{
				m_iTextureIndex = 0;
				m_fIdentity_Ratio = (_float)m_iIdentity_CurrGauge / (_float)m_iIdentity_MaxGauge;
				m_fIdentity_Ratio *= 3.0f;
			}

			break;

		case 2:
			if (100 < m_iIdentity_CurrGauge)
			{
				m_iTextureIndex = 0;
				m_fIdentity_Ratio = ((_float)m_iIdentity_CurrGauge - 100.f) / (_float)m_iIdentity_MaxGauge;
				m_fIdentity_Ratio *= 3.0f;
			}
			break;

		case 3:
			if (200 < m_iIdentity_CurrGauge)
			{
				m_iTextureIndex = 0;
				m_fIdentity_Ratio = ((_float)m_iIdentity_CurrGauge - 200.f) / (_float)m_iIdentity_MaxGauge;
				m_fIdentity_Ratio *= 3.0f;
			}
			break;
		}
		if (1.f <= m_fIdentity_Ratio)
		{
			m_iTextureIndex = 1;
			m_fIdentity_Ratio = 1.f;
		}
	}
}

void CUI_IdentityMG_Gauge::Setting_GaugeScale()
{
	if (m_bSetScale_Gauge)
		return;

	//Gauge
	m_pTransform_Gauge->Set_Scale(Vec3(m_fSizeX * 0.9f, m_fSizeY * 0.9f, 1.f));
	m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	//Gauge Cut
	m_fGaugeCutY = m_fY + ((m_fSizeY* 0.9f) * 0.5f);
	m_pTransform_Cut->Set_Scale(Vec3(32.f, 19.f, 1.f));
	m_pTransform_Cut->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fGaugeCutY + g_iWinSizeY * 0.5f, 0.2f));

	m_bSetScale_Gauge = true;
}

void CUI_IdentityMG_Gauge::Update_Identity_Cut()
{
	if ((0>= m_fIdentity_Ratio)||(1.0f <= m_fIdentity_Ratio))
	{
		m_iGaugeCutTextureIndex = 0;
		m_fGaugeCutOffSet = (m_fSizeY * 0.9f);
	}
	else if ((0.f < m_fIdentity_Ratio)&&(1.0f > m_fIdentity_Ratio))
	{
		m_iGaugeCutTextureIndex = 1;
		m_fGaugeCutOffSet = (m_fIdentity_Ratio * 80.f);
	}
	m_pTransform_Cut->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -(m_fGaugeCutY - m_fGaugeCutOffSet) + (g_iWinSizeY * 0.5f), 0.2f));
}

CUI_IdentityMG_Gauge* CUI_IdentityMG_Gauge::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityMG_Gauge* pInstance = new CUI_IdentityMG_Gauge(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityMG_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityMG_Gauge::Clone(void* pArg)
{
	CUI_IdentityMG_Gauge* pInstance = new CUI_IdentityMG_Gauge(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityMG_Gauge");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityMG_Gauge::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom_Empty);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Frame);
	Safe_Release(m_pTextureCom_Cut);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTransform_Gauge);
	Safe_Release(m_pTransform_Cut);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
