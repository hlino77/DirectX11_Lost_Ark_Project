#include "stdafx.h"
#include "GameInstance.h"
#include "StaticModel.h"
#include "ColliderSphere.h"
#include "ColliderOBB.h"

CStaticModel::CStaticModel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
	: CGameObject(pDevice, pContext, L"StaticModel", eObjType)
{
}

CStaticModel::CStaticModel(const CStaticModel& rhs)
	: CGameObject(rhs)
{
}

HRESULT CStaticModel::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CStaticModel::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iLayer = Desc->iLayer;
	m_szModelName = Desc->strFileName;

	if (FAILED(Ready_Components()))
		return E_FAIL;
	
	m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;

    return S_OK;
}

void CStaticModel::Tick(_float fTimeDelta)
{

}

void CStaticModel::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)	
		return;

	if (m_bRender)
	{
		m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;
		m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
	}
	if (m_szModelName == L"SM_ENV_TCHEXA_Tree_A")
		m_eRenderGroup = CRenderer::RENDERGROUP::INSTANCE_STATIC;
	else
		m_eRenderGroup = CRenderer::RENDERGROUP::RENDER_NONBLEND;
}

HRESULT CStaticModel::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_GlobalWVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render(m_pShaderCom)))
		return E_FAIL;

    return S_OK;
}

HRESULT CStaticModel::Render_ShadowDepth()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return S_OK;

	m_pShaderCom->Push_ShadowWVP();

	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;*/

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, 1)))
			return S_OK;
	}

	return S_OK;
}

HRESULT CStaticModel::Add_ModelComponent(const wstring& strComName)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CStaticModel::Add_Collider()
{
	CSphereCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CSphereCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY_STATICMODEL;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<CSphereCollider*>(pComponent);
	if (nullptr == pCollider)
		return;

	m_StaticColliders.push_back(pCollider);

	Safe_Release(pGameInstance);
}

void CStaticModel::Add_ChildCollider(_uint iIndex)
{
	COBBCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_CHILD;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<COBBCollider*>(pComponent);
	if (nullptr == pCollider)
		return;


	m_StaticColliders[iIndex]->Set_Child(pCollider);

	Safe_Release(pGameInstance);
}

HRESULT CStaticModel::Ready_Components()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Com_Transform */
	CTransform::TRANSFORMDESC		TransformDesc;
	ZeroMemory(&TransformDesc, sizeof(CTransform::TRANSFORMDESC));

	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"), TEXT("Com_Transform"), (CComponent**)&m_pTransformCom, &TransformDesc)))
		return E_FAIL;

	/* For.Com_Renderer */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), TEXT("Com_Renderer"), (CComponent**)&m_pRendererCom)))
		return E_FAIL;

	/* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;
	
	/* For.Com_Shader */
	if (m_pModelCom)
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}
	else
	{
		if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);

	//_vector vScale;
	//vScale.m128_f32[0] = 0.1f;
	//vScale.m128_f32[1] = 0.1f;
	//vScale.m128_f32[2] = 0.1f;

	//m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}


CStaticModel* CStaticModel::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
{
	CStaticModel* pInstance = new CStaticModel(pDevice, pContext, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CStaticModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CStaticModel::Clone(void* pArg)
{
	CStaticModel* pInstance = new CStaticModel(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CStaticModel");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStaticModel::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
