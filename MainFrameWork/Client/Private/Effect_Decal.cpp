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
	, m_protoModel(rhs.m_protoModel)
{
}

HRESULT CEffect_Decal::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	m_protoModel = pDesc->protoModel;

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
	Super::LateTick(fTimeDelta);
}

HRESULT CEffect_Decal::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	_int iMeshCount = m_pModelCom->Get_Meshes().size();
	for (_int i = 0; i < iMeshCount; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "Default")))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Decal::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Model */
	/*if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_protoModel, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;*/

	/* For.Com_Shader */
	/*if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;*/

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
