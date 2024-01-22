#include "stdafx.h"
#include "Effect_Mesh.h"
#include "Client_Defines.h"
#include "GameInstance.h"

CEffect_Mesh::CEffect_Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Super(pDevice, pContext)
{
}

CEffect_Mesh::CEffect_Mesh(const CEffect_Mesh& rhs)
	: Super(rhs)
	, m_protoModel(rhs.m_protoModel)
{
}

HRESULT CEffect_Mesh::Initialize_Prototype(EFFECTDESC* pDesc)
{
	if (FAILED(Super::Initialize_Prototype(pDesc)))
		return E_FAIL;

	m_protoModel = pDesc->protoModel;

	return S_OK;
}

HRESULT CEffect_Mesh::Initialize(void* pArg)
{
	if (FAILED(Super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	return S_OK;
}

void CEffect_Mesh::Tick(_float fTimeDelta)
{
	Super::Tick(fTimeDelta);
}

void CEffect_Mesh::LateTick(_float fTimeDelta)
{
	Super::LateTick(fTimeDelta);
}

HRESULT CEffect_Mesh::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_matCombined, sizeof(Matrix))))
		return E_FAIL;

	_int iMeshCount = m_pModelCom->Get_Meshes().size();
	for (_int i = 0; i < iMeshCount; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, m_strPassName)))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CEffect_Mesh::Ready_Components()
{
	if (FAILED(Super::Ready_Components()))
		return E_FAIL;

	/* For.Com_Model */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Model_") + m_protoModel, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	/* For.Com_Shader */
	if (FAILED(Super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	return S_OK;
}

CEffect_Mesh* CEffect_Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, EFFECTDESC* pDesc)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CEffect_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CEffect_Mesh::Clone(void* pArg)
{
	CEffect_Mesh* pInstance = new CEffect_Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CEffect_Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEffect_Mesh::Free()
{
	Super::Free();
}
