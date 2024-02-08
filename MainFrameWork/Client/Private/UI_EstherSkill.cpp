#include "stdafx.h"
#include "UI_EstherSkill.h"
#include "GameInstance.h"
#include "Player_Gunslinger.h"
#include "Player_Slayer.h"
#include "Player_Destroyer.h"
#include "Player_Bard.h"
#include "Player_Controller_GN.h"
#include "Controller_MG.h"
#include "Controller_WDR.h"
#include "Controller_WR.h"
#include "ServerSessionManager.h"

CUI_EstherSkill::CUI_EstherSkill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}

CUI_EstherSkill::CUI_EstherSkill(const CUI& rhs)
	:CUI(rhs)
{
}

HRESULT CUI_EstherSkill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_EstherSkill::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("UI_EstherSkill");

	m_fSizeX = 324.f;
	m_fSizeY = 205;
	m_fX = 0.f + (m_fSizeX * 0.5f);
	m_fY = 180.f;

	//기본Wnd
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	for (size_t i = 0; i < 3; i++)
	{
		//m_pTransform_IconFrameEffect
		m_pTransform_IconFrameEffect[i]->Set_Scale(Vec3(100.f * 0.8f, 100.f * 0.8f, 1.f));
		m_pTransform_IconFrameEffect[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((70.f + 80.f * (i)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));

		//m_pTransform_IconFrame
		m_pTransform_IconFrame[i]->Set_Scale(Vec3(80.f * 0.8f, 80.f * 0.8f, 1.f));
		m_pTransform_IconFrame[i]->Set_State(CTransform::STATE_POSITION,
			Vec3((70.f + 80.f * (i)) - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	}

	//m_pTransform_GaugeFrame
	m_pTransform_GaugeFrame->Set_Scale(Vec3(263.f, 28.f, 1.f));
	m_pTransform_GaugeFrame->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 15.f) - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY * 0.25f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_Gauge
	m_pTransform_Gauge->Set_Scale(Vec3(240.f, 24.f, 1.f));
	m_pTransform_Gauge->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 15.f) - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY * 0.25f) + g_iWinSizeY * 0.5f, 0.2f));
	//m_pTransform_GaugeFrameEffect
	m_pTransform_GaugeFrameEffect->Set_Scale(Vec3(257.f, 33.f, 1.f));
	m_pTransform_GaugeFrameEffect->Set_State(CTransform::STATE_POSITION,
		Vec3((m_fX - 15.f) - g_iWinSizeX * 0.5f, -(m_fY + m_fSizeY*0.25f) + g_iWinSizeY * 0.5f, 0.2f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_fMaxGauge = 100.f;
	m_fCurrGauge = m_fMaxGauge;
	m_fGaugeRatio = m_fCurrGauge / m_fMaxGauge;

	m_pPartyLeader = CServerSessionManager::GetInstance()->Get_Player();

	m_vecUIParts.push_back(this);

	return S_OK;
}

void CUI_EstherSkill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Update_EstherGauge(fTimeDelta);
}

void CUI_EstherSkill::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
	Update_UseEshterSkill();
	if (m_fCurrGauge == m_fMaxGauge)
		m_bMaxGauge = true;
	else
		m_bMaxGauge = false;

	Update_ShineEffect(fTimeDelta);
}

HRESULT CUI_EstherSkill::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameEffectL()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameEffectC()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameEffectR()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameL()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameC()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_IconFrameR()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if(FAILED(Bind_ShaderResources_GaugeFrame()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_GaugeFrameEffect()))
		return E_FAIL;
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources_Gauge()))
		return E_FAIL;
	m_pShaderCom->Begin(16);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_EstherSkill::Update_UseEshterSkill()
{
	if (nullptr == m_pPartyLeader)
		return;

	if (TEXT("Gunslinger") == m_pPartyLeader->Get_ObjectTag())
	{
		m_bSkillUse = static_cast<CPlayer_Gunslinger*>(m_pPartyLeader)->Get_GN_Controller()->Is_EstherSkill();
	}
	else if (TEXT("WR") == m_pPartyLeader->Get_ObjectTag())
	{
		m_bSkillUse =static_cast<CPlayer_Slayer*>(m_pPartyLeader)->Get_WR_Controller()->Is_EstherSkill();
	}
	else if (TEXT("WDR") == m_pPartyLeader->Get_ObjectTag())
	{
		m_bSkillUse = static_cast<CPlayer_Destroyer*>(m_pPartyLeader)->Get_WDR_Controller()->Is_EstherSkill();
	}
	else if (TEXT("SP") == m_pPartyLeader->Get_ObjectTag())
	{
		m_bSkillUse = static_cast<CPlayer_Bard*>(m_pPartyLeader)->Get_MG_Controller()->Is_EstherSkill();
	}

}

void CUI_EstherSkill::Update_EstherGauge(_float fTimeDelta)
{
	if (m_bSkillUse)
	{
		m_tLerp.Init_Lerp(2.f, m_fMaxGauge, 0.f, LERP_MODE::SMOOTHSTEP);//지속시간, 시작값, 끝나는 값, 러프모드 : 
		m_bSkillUse = false;
	}

	if (true == m_tLerp.bActive)
	{
		m_fCurrGauge = m_tLerp.Update_Lerp(fTimeDelta);
		m_fGaugeRatio = m_fCurrGauge / m_fMaxGauge;
	}
}

void CUI_EstherSkill::Update_ShineEffect(_float  fTimeDelta)
{
	if (!m_bMaxGauge)
	{
		if (0.f < m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha -= fTimeDelta;
			m_fGaugeEffectAlpha -= fTimeDelta;
		}
		if (0.f > m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha = 0.f;
			m_fGaugeEffectAlpha = 0.f;
		}
		return;
	}

	if (!m_bDecreaseEffect)
	{
		if (1.f >= m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha += fTimeDelta;
			m_fGaugeEffectAlpha += fTimeDelta;
		}
		if (1.f < m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha = 1.f;
			m_fGaugeEffectAlpha = 1.f;
			m_bDecreaseEffect = true;
		}
	}

	else if (m_bDecreaseEffect)
	{
		if (0.f < m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha -= fTimeDelta;
			m_fGaugeEffectAlpha -= fTimeDelta;
		}
		if (0.f > m_fFrameEffectAlpha)
		{
			m_fFrameEffectAlpha = 0.f;
			m_fGaugeEffectAlpha = 0.f;
			m_bDecreaseEffect = false;
		}
	}
}

HRESULT CUI_EstherSkill::Ready_Components()
{
	if (FAILED(__super::Ready_Components()))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_EstherSkillWnd"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_IconFrame_Effect"),
		TEXT("Com_Texture_IconFrameEffect"), (CComponent**)&m_pTexture_IconFrameEffect)))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_EstherIconFrame"),
		TEXT("Com_Texture_IconFrame"), (CComponent**)&m_pTexture_IconFrame)))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_Esther_GaugeFrame"),
		TEXT("Com_Texture_GaugeFrame"), (CComponent**)&m_pTexture_GaugeFrame)))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_Esther_Gauge"),
		TEXT("Com_Texture_Gauge"), (CComponent**)&m_pTexture_Gauge)))
		return E_FAIL;
	/*m_pTexture_PartyHPWnd*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ValtanRaid_Max_GaugeEffect"),
		TEXT("Com_Texture_GaugeFrameEffect"), (CComponent**)&m_pTexture_GaugeFrameEffect)))
		return E_FAIL;

	for (size_t i = 0; i < 3; i++)
	{
		/* m_pTransform_IconFrameEffect */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_IconFrameEffect") + to_wstring(i), (CComponent**)&m_pTransform_IconFrameEffect[i])))
			return E_FAIL;
		/* m_pTransform_IconFrame */
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
			TEXT("Com_Transform_IconFrame")+ to_wstring(i), (CComponent**)&m_pTransform_IconFrame[i])))
			return E_FAIL;
	}
	
	/* m_pTransform_GaugeFrame */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_GaugeFrame"), (CComponent**)&m_pTransform_GaugeFrame)))
		return E_FAIL;
	/* m_pTransform_GaugeFrameEffect */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_GaugeFrameEffect"), (CComponent**)&m_pTransform_GaugeFrameEffect)))
		return E_FAIL;
	/*m_pTransform_Gauge*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform_Gauge"), (CComponent**)&m_pTransform_Gauge)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources()
{
	if (FAILED(__super::Bind_ShaderResources()))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameEffectL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrameEffect[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fFrameEffectAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrameEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameEffectC()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrameEffect[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fFrameEffectAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrameEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameEffectR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrameEffect[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fFrameEffectAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrameEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameL()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrame[0]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 0);
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameC()
{

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrame[1]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 1);
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_IconFrameR()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_IconFrame[2]->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_IconFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture", 2);
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_GaugeFrame()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeFrame->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTexture_GaugeFrame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_GaugeFrameEffect()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_GaugeFrameEffect->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fGaugeEffectAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	m_pTexture_GaugeFrameEffect->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

HRESULT CUI_EstherSkill::Bind_ShaderResources_Gauge()
{
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransform_Gauge->Get_WorldMatrix())))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fGaugeRatio, sizeof(_float))))
		return E_FAIL;
	m_pTexture_Gauge->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	return S_OK;
}

CUI_EstherSkill* CUI_EstherSkill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_EstherSkill* pInstance = new CUI_EstherSkill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_EstherSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_EstherSkill::Clone(void* pArg)
{
	CUI_EstherSkill* pInstance = new CUI_EstherSkill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_EstherSkill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_EstherSkill::Free()
{
	__super::Free();
	Safe_Release(m_pTexture_IconFrameEffect);
	Safe_Release(m_pTexture_IconFrame);
	Safe_Release(m_pTexture_GaugeFrame);
	Safe_Release(m_pTexture_Gauge);
	Safe_Release(m_pTexture_GaugeFrameEffect);

	for (size_t i = 0; i < 3; i++)
	{
		if(nullptr != m_pTransform_IconFrameEffect[i])
			Safe_Release(m_pTransform_IconFrameEffect[i]);
		if (nullptr != m_pTransform_IconFrame[i])
			Safe_Release(m_pTransform_IconFrame[i]);
	}

	Safe_Release(m_pTransform_GaugeFrame);
	Safe_Release(m_pTransform_Gauge);
	Safe_Release(m_pTransform_GaugeFrameEffect);
}
