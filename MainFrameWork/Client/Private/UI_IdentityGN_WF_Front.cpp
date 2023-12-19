#include "stdafx.h"
#include "UI_IdentityGN_WF_Front.h"
#include "GameInstance.h"

CUI_IdentityGN_WF_Front::CUI_IdentityGN_WF_Front(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityGN_WF_Front::CUI_IdentityGN_WF_Front(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN_WF_Front::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN_WF_Front::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityGN_WeaponFrame_Front");

	m_fSizeX = 400.f;
	m_fSizeY = 268.f;
	m_fX = 12.f;
	m_fY = 543.f;
	m_fAlpha = 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
	return S_OK;
}

void CUI_IdentityGN_WF_Front::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if (KEY_TAP(KEY::Z))
	{
		m_bChangeIcon = false;
		m_bTapKey = false;
	}
	else if (KEY_TAP(KEY::X))
	{
		m_bChangeIcon = false;
		m_bTapKey = true;
	}

	Change_Icon(fTimeDelta);
}

void CUI_IdentityGN_WF_Front::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);
}

HRESULT CUI_IdentityGN_WF_Front::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(2);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CUI_IdentityGN_WF_Front::Change_Icon(_float fTimeDelta)
{
	if (!m_bChangeIcon)
	{
		if (!m_bTapKey)
		{
			if ((380.f > m_fAngle) && (!m_bReturn))
			{
				m_fAngle += 20.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(m_fAngle));
			}
			else if ((380.f <= m_fAngle) && (!m_bReturn))
				m_bReturn = true;
			if ((m_bReturn) && (360.f < m_fAngle))
			{
				m_fAngle -= 1.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(m_fAngle));
			}
			else if ((m_bReturn) && (360.f >= m_fAngle))
			{
				m_bReturn = false;
				m_bChangeIcon = true;
				m_fAngle = 0.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(0.f));
			}
		}
		else if (m_bTapKey)
		{
			if ((380.f > m_fAngle) && (!m_bReturn))
			{
				m_fAngle += 20.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-m_fAngle));
			}
			else if ((380.f <= m_fAngle) && (!m_bReturn))
				m_bReturn = true;
			if ((m_bReturn) && (360.f < m_fAngle))
			{
				m_fAngle -= 1.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(-m_fAngle));
			}
			else if ((m_bReturn) && (360.f >= m_fAngle))
			{
				m_bReturn = false;
				m_bChangeIcon = true;
				m_fAngle = 0.f;
				m_pTransformCom->Rotation(Vec3(0.f, 0.f, 1.f), XMConvertToRadians(0.f));
			}
		}
	}
}

HRESULT CUI_IdentityGN_WF_Front::Ready_Components()
{
	__super::Ready_Components();

	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_IdentityGN_WeaponFrame_Front"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CUI_IdentityGN_WF_Front::Bind_ShaderResources()
{
	if (__super::Bind_ShaderResources())
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

	return S_OK;
}

CUI_IdentityGN_WF_Front* CUI_IdentityGN_WF_Front::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN_WF_Front* pInstance = new CUI_IdentityGN_WF_Front(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN_WF_Front");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN_WF_Front::Clone(void* pArg)
{
	CUI_IdentityGN_WF_Front* pInstance = new CUI_IdentityGN_WF_Front(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN_WF_Front");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN_WF_Front::Free()
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
