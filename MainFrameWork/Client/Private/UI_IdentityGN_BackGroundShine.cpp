#include "stdafx.h"
#include "UI_IdentityGN_BackGroundShine.h"
#include "GameInstance.h"

CUI_IdentityGN_BackGroundShine::CUI_IdentityGN_BackGroundShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_BackGroundShine::CUI_IdentityGN_BackGroundShine(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_BackGroundShine::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_BackGroundShine::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_BackGroundShine");

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY - m_fSizeY*0.5f;
	m_fAlpha = 0.f;
	m_vColor = { 3.f, 3.f, 3.f, 1.f };
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.2f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	return S_OK;
}

void CUI_IdentityGN_BackGroundShine::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	Change_Weapon(fTimeDelta);
}

void CUI_IdentityGN_BackGroundShine::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_BackGroundShine::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(2);
	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_IdentityGN_BackGroundShine::Change_Weapon(_float fTimeDelta)
{
	if (!m_bChange)
		return;
	if (!m_bHidden)
	{
		if(0.6f > m_fAlpha)
			m_fAlpha += 4.f * fTimeDelta;
		if(0.6f <= m_fAlpha)
			m_bHidden = true;
	}
	else if (m_bHidden)
	{
		if (0 < m_fAlpha)
			m_fAlpha -= 2.f * fTimeDelta;
		if (0 >= m_fAlpha)
		{
			m_bHidden = false;
			m_fAlpha = 0.f;
			m_bChange = false;
		}
	}
	
}

HRESULT CUI_IdentityGN_BackGroundShine::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_BackGround_Shine"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_BackGroundShine::Bind_ShaderResources()
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

CUI_IdentityGN_BackGroundShine* CUI_IdentityGN_BackGroundShine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_BackGroundShine* pInstance = new CUI_IdentityGN_BackGroundShine(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_BackGroundShine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_BackGroundShine::Clone(void* pArg)
{
	CUI_IdentityGN_BackGroundShine* pInstance = new CUI_IdentityGN_BackGroundShine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_BackGroundShine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_BackGroundShine::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
