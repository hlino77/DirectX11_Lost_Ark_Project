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
	if (FAILED(Ready_Proto_InstanceBuffer()))
		return E_FAIL;

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
		if (m_bInstance)
			m_pRendererCom->Add_InstanceRenderGroup(m_eRenderGroup, this);
		else
			m_pRendererCom->Add_RenderGroup(m_eRenderGroup, this);
	}

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

HRESULT CStaticModel::Render_Instance(_uint iSize)
{
	if (nullptr == m_pModelCom || nullptr == m_pShaderCom)
		return E_FAIL;

	if (FAILED(m_pInstanceShader->Push_GlobalVP()))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render_Instance(m_pInstanceBuffer, iSize, m_pInstanceShader, sizeof(Matrix))))
		return E_FAIL;


	//Safe_Release(pGameInstance);

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

void CStaticModel::Add_InstanceData(_uint iSize, _uint& iIndex)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(m_pInstanceValue->GetValue());

	(*pInstanceValue)[iIndex] = m_pTransformCom->Get_WorldMatrix();

	if (iSize - 1 == iIndex)
	{
		ThreadManager::GetInstance()->EnqueueJob([=]()
			{
				Ready_Instance_For_Render(iSize);
			});
	}
	else
		++iIndex;
}

HRESULT CStaticModel::Ready_Proto_InstanceBuffer()
{
	/* For.Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		TEXT("Com_InstanceShader"), (CComponent**)&m_pInstanceShader)))
		return E_FAIL;

	m_iMaxInstanceCount = 100;

	m_pInstanceValue = new tagTypeLessData<vector<Matrix>>();

	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(m_pInstanceValue->GetValue());
	
	pInstanceValue->resize(m_iMaxInstanceCount, XMMatrixIdentity());

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));

		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(Matrix) * m_iMaxInstanceCount;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(Matrix);

		D3D11_SUBRESOURCE_DATA		InitialData;

		InitialData.pSysMem = pInstanceValue->data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pInstanceBuffer)))
			return E_FAIL;
	}
}

HRESULT CStaticModel::Ready_Instance_For_Render(_uint iSize)
{
	vector<Matrix>* pInstanceValue = static_cast<vector<Matrix>*>(m_pInstanceValue->GetValue());

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	ID3D11DeviceContext* pContext = CGameInstance::GetInstance()->Get_BeforeRenderContext();

	if (pContext == nullptr)
		return E_FAIL;

	if(FAILED(pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource)))
		return E_FAIL;

	memcpy(SubResource.pData, pInstanceValue->data(), sizeof(Matrix) * iSize);

	pContext->Unmap(m_pInstanceBuffer, 0);

	CGameInstance::GetInstance()->Release_BeforeRenderContext(pContext);
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
