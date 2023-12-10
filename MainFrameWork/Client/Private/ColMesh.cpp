#include "stdafx.h"
#include "GameInstance.h"
#include "ColMesh.h"
#include "ColliderOBB.h"
#include "CollisionManager.h"

CColMesh::CColMesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
	: CGameObject(pDevice, pContext, L"ColMesh", eObjType)
{
}

CColMesh::CColMesh(const CColMesh& rhs)
	: CGameObject(rhs)
{
}

HRESULT CColMesh::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CColMesh::Initialize(void* pArg)
{
	MODELDESC* Desc = static_cast<MODELDESC*>(pArg);
	m_strObjectTag = Desc->strFileName;
	m_iLayer = Desc->iLayer;
	m_szModelName = Desc->strFileName;

	if (FAILED(Ready_Components()))
		return E_FAIL;


    return S_OK;
}

void CColMesh::Tick(_float fTimeDelta)
{
}

void CColMesh::LateTick(_float fTimeDelta)
{
	if (nullptr == m_pRendererCom)
		return;


	//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CColMesh::Render()
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return S_OK;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;


	_uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;


		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/


		if (FAILED(m_pModelCom->Render(m_pShaderCom, i)))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);



	for (auto Collider : m_StaticColliders)
	{
		Collider->DebugRender();

		if (Collider->Get_Child())
			Collider->Get_Child()->DebugRender();
	}


    return S_OK;
}

HRESULT CColMesh::Add_ModelComponent(const wstring& strComName)
{
	if (FAILED(__super::Add_Component(LEVEL_STATIC, strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;

	return S_OK;
}

void CColMesh::Add_Collider()
{
	CSphereCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CSphereCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_BODY;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<CSphereCollider*>(pComponent);
	if (nullptr == pCollider)
		return;

	CCollisionManager::GetInstance()->Add_Colider(pCollider);

	m_StaticColliders.push_back(pCollider);

	Safe_Release(pGameInstance);

}

void CColMesh::Add_ChildCollider(_uint iIndex)
{
	COBBCollider* pCollider = nullptr;


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	CCollider::ColliderInfo tInfo;
	tInfo.m_bActive = true;
	tInfo.m_iLayer = (_uint)LAYER_COLLIDER::LAYER_END;

	CComponent* pComponent = pGameInstance->Clone_Component(this, LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"), &tInfo);
	if (nullptr == pComponent)
		return;

	pCollider = dynamic_cast<COBBCollider*>(pComponent);
	if (nullptr == pCollider)
		return;


	m_StaticColliders[iIndex]->Set_Child(pCollider);

	Safe_Release(pGameInstance);
}

HRESULT CColMesh::Ready_Components()
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

	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"), TEXT("Com_Shader"), (CComponent**)&m_pShaderCom)))
		return E_FAIL;

	///* For.Com_Model */
	wstring strComName = L"Prototype_Component_Model_" + m_strObjectTag;
	if (FAILED(__super::Add_Component(pGameInstance->Get_CurrLevelIndex(), strComName, TEXT("Com_Model"), (CComponent**)&m_pModelCom)))
		return E_FAIL;



	Safe_Release(pGameInstance);

	//_vector vScale;
	//vScale.m128_f32[0] = 0.1f;
	//vScale.m128_f32[1] = 0.1f;
	//vScale.m128_f32[2] = 0.1f;

	//m_pTransformCom->Set_Scale(vScale);

    return S_OK;
}


CColMesh* CColMesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, OBJ_TYPE eObjType)
{
	CColMesh* pInstance = new CColMesh(pDevice, pContext, eObjType);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CColMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CColMesh::Clone(void* pArg)
{
	CColMesh* pInstance = new CColMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CColMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CColMesh::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pRendererCom);
	Safe_Release(m_pTransformCom);
}
