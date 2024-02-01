#include "stdafx.h"
#include "UI_WDRIdentity_Hammer.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "ServerSessionManager.h"
#include "Controller_WDR.h"
#include "Player_Destroyer.h"

CUI_WDRIdentity_Hammer::CUI_WDRIdentity_Hammer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WDRIdentity_Hammer::CUI_WDRIdentity_Hammer(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WDRIdentity_Hammer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity_Hammer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityWDR_Hammer");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_fAlpha = 1.f;
	m_vColor = { 3.f, 3.f, 3.f, 1.f };
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_pPlayer = CServerSessionManager::GetInstance()->Get_Player();
	if (nullptr != m_pPlayer)
		m_bIdentity_On = static_cast<CPlayer_Destroyer*>(m_pPlayer)->
		Get_WDR_Controller()->Is_Identity();

	return S_OK;
}

void CUI_WDRIdentity_Hammer::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	Update_Identity_Hammer(fTimeDelta);
}

void CUI_WDRIdentity_Hammer::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_WDRIdentity_Hammer::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);

	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_WDRIdentity_Hammer::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WDR_Identity_Hammer"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_WDRIdentity_Hammer::Bind_ShaderResources()
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

void CUI_WDRIdentity_Hammer::Identity_Hammer_Normal(_float fTimeDelta)
{
	if ((_uint)HAMMER_NORMAL != m_iCurrHammerState)
		return;

	m_fSizeX = 100.f;
	m_fSizeY = 100.f;
	m_fX = 800.f;
	m_fY = 840.f;

	if (m_bIdentity_On)
	{
		m_fSizeX = 100.f;
		m_fSizeY = 120.f;
		m_fX = 800.f;
		m_fY = 845.f;
		m_iCurrHammerState = HAMMER_TRANSFORM_ON;
	}

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
}

void CUI_WDRIdentity_Hammer::Identity_Hammer_Transform_On(_float fTimeDelta)
{
	if ((_uint)HAMMER_TRANSFORM_ON != m_iCurrHammerState)
		return;

	if (m_bColorDecrease)
	{
		m_fSizeX -= 70.f * fTimeDelta;
		m_fSizeY -= 70.f * fTimeDelta;
		m_vColor -= Vec4(10.f * fTimeDelta, 10.f * fTimeDelta, 10.f * fTimeDelta, 0.f);
	}
	else if (!m_bColorDecrease)
	{
		m_fSizeX += 70.f * fTimeDelta;
		m_fSizeY += 70.f * fTimeDelta;
		m_vColor += Vec4(10.f * fTimeDelta, 10.f * fTimeDelta, 10.f * fTimeDelta, 0.f);
	}

	if (800.f < m_fY)
		m_fY -= 150.f * fTimeDelta;
	if (800.f >= m_fY)
		m_fY = 800.f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	if (1.f >= m_vColor.x)
	{
		m_fSizeX = 100.f;
		m_vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		m_iCurrHammerState = (_uint)HAMMER_ON;
		m_bColorDecrease = false;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));

	}
	else if (5.f <= m_vColor.x)
		m_bColorDecrease = true;
}

void CUI_WDRIdentity_Hammer::Identity_Hammer_On(_float fTimeDelta)
{
	if ((_uint)HAMMER_ON != m_iCurrHammerState)
		return;

	if (!m_bIdentity_On)
	{
		m_bColorDecrease = false;
		m_iCurrHammerState = HAMMER_TRANSFORM_OFF;
	}
}

void CUI_WDRIdentity_Hammer::Identity_Hammer_Transform_Off(_float fTimeDelta)
{
	if ((_uint)HAMMER_TRANSFORM_OFF != m_iCurrHammerState)
		return;

	if (m_bColorDecrease)
	{
		if (100.f < m_fSizeX)
			m_fSizeX -= 80.f * fTimeDelta;
		else if (100.f >= m_fSizeX)
			m_fSizeX = 100.f;

		if (100.f < m_fSizeY)
			m_fSizeY -= 70.f * fTimeDelta;
		else if (100.f >= m_fSizeY)
			m_fSizeY = 100.f;

		if (840.f > m_fY)
			m_fY += 400.f * fTimeDelta;
		else if (840.f < m_fY)
			m_fY = 840.f;

		m_vColor -= Vec4(10.f * fTimeDelta, 10.f * fTimeDelta, 10.f * fTimeDelta, 0.f);
	}
	else if (!m_bColorDecrease)
	{
		m_fSizeX += 70.f * fTimeDelta;
		m_fSizeY += 70.f * fTimeDelta;
		m_vColor += Vec4(10.f * fTimeDelta, 10.f * fTimeDelta, 10.f * fTimeDelta, 0.f);
	}

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;

	if (1.f >= m_vColor.x)
	{
		m_vColor = Vec4(1.f, 1.f, 1.f, 1.f);
		m_iCurrHammerState = (_uint)HAMMER_NORMAL;
		m_bColorDecrease = false;
		m_fY = 840.f;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
	}
	else if (5.f <= m_vColor.x)
	{
		m_iTextureIndex = 0;
		m_fY = 795.f;
		m_fSizeY = 100.f;
		m_bColorDecrease = true;

		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
	}
}

void CUI_WDRIdentity_Hammer::Update_Identity_Hammer(_float fTimeDelta)
{
	if (nullptr != m_pPlayer)
	{
		m_bIdentity_On = static_cast<CPlayer_Destroyer*>(m_pPlayer)->
			Get_WDR_Controller()->Is_In_Identity();

		if (m_bIdentity_On)
			m_iTextureIndex = 1;
	}

	Identity_Hammer_Normal(fTimeDelta);
	Identity_Hammer_Transform_On(fTimeDelta);
	Identity_Hammer_On(fTimeDelta);
	Identity_Hammer_Transform_Off(fTimeDelta);

}

CUI_WDRIdentity_Hammer* CUI_WDRIdentity_Hammer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WDRIdentity_Hammer* pInstance = new CUI_WDRIdentity_Hammer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WDRIdentity_Hammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WDRIdentity_Hammer::Clone(void* pArg)
{
	CUI_WDRIdentity_Hammer* pInstance = new CUI_WDRIdentity_Hammer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WDRIdentity_Hammer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WDRIdentity_Hammer::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
