#include "stdafx.h"
#include "Esther_Scene.h"
#include "GameInstance.h"
#include "Text_Manager.h"

CEsther_Scene::CEsther_Scene(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject(pDevice, pContext, L"Esther_Scene", OBJ_TYPE::UI)
{

}

CEsther_Scene::CEsther_Scene(const CGameObject& rhs)
	: CGameObject(rhs)
{

}

HRESULT CEsther_Scene::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CEsther_Scene::Initialize(void* pArg)
{
	ESTHERSCENEDESC* pDesc = static_cast<ESTHERSCENEDESC*>(pArg);
	m_fStartFrame = pDesc->fStartFrame;
	m_fFinalFrame = pDesc->fFinalFrame;
	m_fFrameSpeed = pDesc->fFrameSpeed;
	m_strTexutre = pDesc->strTexutre;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fSizeX = g_iWinSizeX;
	m_fSizeY = g_iWinSizeY;
	m_fX = g_iWinSizeX * 0.5f;
	m_fY = g_iWinSizeY * 0.5f;

	m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
	m_pTransformCom->Set_State(CTransform::STATE_POSITION,
		Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

	m_ViewMatrix = XMMatrixIdentity();
	m_ProjMatrix = XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f);

	m_fFrame = m_fStartFrame;

	return S_OK;
}

void CEsther_Scene::Tick(_float fTimeDelta)
{
	if (true == m_bPlayFrame)
	{
		if (m_fFinalFrame > m_fFrame)
		{
			m_fFrame += (fTimeDelta * m_fFrameSpeed);
		}
		if (m_fFinalFrame <= m_fFrame)
		{
			m_fFrame = m_fStartFrame;
			m_bPlayFrame = false;
		}
	}
}

void CEsther_Scene::LateTick(_float fTimeDelta)
{
	if (true == m_bPlayFrame)
		m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_ESHTER, this);
}

HRESULT CEsther_Scene::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();

	return S_OK;
}

void CEsther_Scene::Play_Frame()
{
	m_bPlayFrame = true;
	m_fFrame = m_fStartFrame;
}

HRESULT CEsther_Scene::Ready_Components()
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
	wstring strTexture = TEXT("Prototype_Component_Texture_") + m_strTexutre;

	if (FAILED(__super::Add_Component(LEVEL_STATIC, strTexture,
		TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
		return E_FAIL;

	/* Com_Transform */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		TEXT("Com_Transform"), (CComponent**)&m_pTransformCom)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEsther_Scene::Bind_ShaderResources()
{
	/* 셰이더 전역변수로 던져야 할 값들을 던지자. */
	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_float fAlpha = 1.0f;
	if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &fAlpha, sizeof(_float))))
		return E_FAIL;

	m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", (_uint)m_fFrame);

	return S_OK;
}

CEsther_Scene* CEsther_Scene::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CEsther_Scene* pInstance = new CEsther_Scene(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEsther_Scene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEsther_Scene::Clone(void* pArg)
{
	CEsther_Scene* pInstance = new CEsther_Scene(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEsther_Scene");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEsther_Scene::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
