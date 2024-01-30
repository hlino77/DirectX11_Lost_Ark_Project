#include "stdafx.h"
#include "BackGround_MainLogo.h"
#include "GameInstance.h"

CBackGround_MainLogo::CBackGround_MainLogo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CGameObject(pDevice, pContext, L"MainLogo", OBJ_TYPE::UI)
{

}

CBackGround_MainLogo::CBackGround_MainLogo(const CGameObject & rhs)
	: CGameObject(rhs)
{

}

HRESULT CBackGround_MainLogo::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBackGround_MainLogo::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 1.0f));

	m_ViewMatrix = XMMatrixIdentity();
	m_ProjMatrix = XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	return S_OK;
}

void CBackGround_MainLogo::Tick(_float fTimeDelta)
{
	if (35 > m_fFrame)
		m_fFrame += (fTimeDelta * 18.f);
}

void CBackGround_MainLogo::LateTick(_float fTimeDelta)
{
	m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CBackGround_MainLogo::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom_BackGround->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;
	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame);
	m_pShaderCom->Begin(0);
	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CBackGround_MainLogo::Ready_Components()
{
	/* Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"),
		TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), (CComponent**)&m_pVIBufferCom)))
		return E_FAIL;


	/* Com_Texture*/
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LostArk_MainLogo"),
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MainLogo_BackGround"),
		TEXT("Com_Texture_BackGround"), (CComponent**)&m_pTextureCom_BackGround)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CBackGround_MainLogo::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	//if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &IdentityMatrix)))
	//	return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = 1.0f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;
	return S_OK;
}

CBackGround_MainLogo * CBackGround_MainLogo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CBackGround_MainLogo*	pInstance = new CBackGround_MainLogo(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject * CBackGround_MainLogo::Clone(void* pArg)
{
	CBackGround_MainLogo*	pInstance = new CBackGround_MainLogo(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CBackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround_MainLogo::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom_BackGround);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
