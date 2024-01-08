#include "stdafx.h"
#include "UI_IdentityGN_BackGroundWing.h"
#include "GameInstance.h"

CUI_IdentityGN_BackGroundFWing::CUI_IdentityGN_BackGroundFWing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_BackGroundFWing::CUI_IdentityGN_BackGroundFWing(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_BackGroundFWing::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_BackGroundFWing::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_BackGroundWing");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY - m_fSizeY*0.5f;
	m_fAlpha = 1.f;
	m_vColor = { 3.f, 3.f, 3.f, 1.f };
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentityGN_BackGroundFWing::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

	if ((m_bChange)&&(!m_bUpState))
	{
		if (10 >= m_fDistance)
		{
			m_fDistance += 500.0f * fTimeDelta;
			m_fY += 500.0f * fTimeDelta;

			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
		}
		else if (10 <= m_fDistance)
			m_bUpState = true;
	}
	else if ((m_bChange) && (m_bUpState))
	{
		m_fDistance -= 500.0f * fTimeDelta;
		m_fY -= 500.0f * fTimeDelta;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
		if (0 >= m_fDistance)
		{
			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
			m_bUpState = false;
			m_bChange = false;
		}
	}
	//Change_Weapon();
}

void CUI_IdentityGN_BackGroundFWing::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_BackGroundFWing::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_IdentityGN_BackGroundFWing::Change_Weapon()
{
	if (!m_bChange)
		return;

	if (!m_bUpState)
	{
		if (10 >= m_fDistance)
		{
			m_fDistance += 2.f;
			m_fY += 2.f;

			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
		}
		else if (10 <= m_fDistance)
			m_bUpState = true;
	}
	else if (m_bUpState)
	{
		m_fDistance -= 2.f;
		m_fY -= 2.f;
		m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
		m_pTransformCom->Set_State(CTransform::STATE_POSITION,
			Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
		if (0 >= m_fDistance)
		{
			m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
			m_pTransformCom->Set_State(CTransform::STATE_POSITION,
				Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.2f));;
			m_bUpState = false;
			m_bChange = false;
		}
	}
}

HRESULT CUI_IdentityGN_BackGroundFWing::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Wing"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_BackGroundFWing::Bind_ShaderResources()
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

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_IdentityGN_BackGroundFWing* CUI_IdentityGN_BackGroundFWing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_BackGroundFWing* pInstance = new CUI_IdentityGN_BackGroundFWing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_BackGroundFWing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_BackGroundFWing::Clone(void* pArg)
{
	CUI_IdentityGN_BackGroundFWing* pInstance = new CUI_IdentityGN_BackGroundFWing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_BackGroundFWing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_BackGroundFWing::Free()
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
