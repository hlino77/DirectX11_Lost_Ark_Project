#include "stdafx.h"
#include "UI_IdentityMG_Skill.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Controller_MG.h"
#include "Player_Bard.h"

CUI_IdentityMG_Skill::CUI_IdentityMG_Skill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityMG_Skill::CUI_IdentityMG_Skill(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityMG_Skill::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityMG_Skill::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		_uint* strIndex = static_cast<_uint*>(pArg);
		m_iKey = *strIndex;
		m_strUITag = TEXT("IdentityMG_Skill");
		m_strUITag += to_wstring(*strIndex);
	}
	else
		m_strUITag = TEXT("IdentityMG_Skill");

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = 200.f;
	m_fSizeY = 200.f;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;
	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - (g_iWinSizeX * 0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));
	
	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

	m_fCoolMaxTime = 1.f;

	return S_OK;
}

void CUI_IdentityMG_Skill::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	m_fResultCool = m_fCoolMaxTime - m_fCurrCool;	
}

void CUI_IdentityMG_Skill::LateTick(_float fTimeDelta)
{
	__super::LateTick(fTimeDelta);

	if (m_fCoolMaxTime == m_fResultCool)
	{
		m_fCoolAngle = XM_PI;
	}
	else
	{
		m_fCoolRatio = 1.0f - (m_fResultCool / m_fCoolMaxTime);
		m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
	}

}

HRESULT CUI_IdentityMG_Skill::Render()
{
	//Skill
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pShaderCom->Begin(8);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CUI_IdentityMG_Skill::Ready_Components()
{
	__super::Ready_Components();	

	if (0 == m_iKey)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Serenade_of_Courage"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	else if (1 == m_iKey)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Bard_Identity_Serenade_of_Salvation"),
			TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
			return E_FAIL;
	}
	return S_OK;
}

HRESULT CUI_IdentityMG_Skill::Bind_ShaderResources()
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
	if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_iTextureIndex);

	return S_OK;
}

CUI_IdentityMG_Skill* CUI_IdentityMG_Skill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityMG_Skill* pInstance = new CUI_IdentityMG_Skill(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityMG_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityMG_Skill::Clone(void* pArg)
{
	CUI_IdentityMG_Skill* pInstance = new CUI_IdentityMG_Skill(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityMG_Skill");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityMG_Skill::Free()
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
