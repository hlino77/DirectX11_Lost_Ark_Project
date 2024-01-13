#include "stdafx.h"
#include "Effect_Decal.h"
#include "Client_Defines.h"
#include "GameInstance.h"

CEffect_Decal::CEffect_Decal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Decal::CEffect_Decal(const CEffect_Decal& rhs)
	: Super(rhs)
{
}

HRESULT CEffect_Decal::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Decal::Initialize(void* pArg)
{
	if (FAILED(Super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Decal::Tick(_float fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CEffect_Decal::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;

	if (m_bRender)
	{
		if (FAILED(m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_EFFECT, this)))
			__debugbreak();
	}
}

HRESULT CEffect_Decal::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	Matrix matCombinedInv = m_matCombined.Invert();

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformInverse", &matCombinedInv, sizeof(Matrix))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_PROJ))) ||
		FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrixInv", &m_pGameInstance->Get_TransformMatrixInverse(CPipeLine::TRANSFORMSTATE::D3DTS_VIEW))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")) ||
		FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Normal"), "g_NormalTarget")) ||
		FAILED(m_pGameInstance->Bind_SRV(m_pShaderCom, TEXT("Target_Properties"), "g_PropertiesTarget")))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(m_strPassName)))
		return E_FAIL;
	if (FAILED(m_pBuffer->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT CEffect_Decal::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Buffer */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"), TEXT("Com_Buffer"), (CComponent**)&m_pBuffer)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectDecal"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Decal* CEffect_Decal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc)
{
	CEffect_Decal* pInstance = new CEffect_Decal(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CEffect_Decal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Decal::Clone(void* pArg)
{
	CEffect_Decal* pInstance = new CEffect_Decal(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Decal");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Decal::Free()
{
	Super::Free();
}
