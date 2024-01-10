#include "..\Public\PartObject.h"
#include "Transform.h"
#include "Model.h"
#include "Shader.h"

CPartObject::CPartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, wstring strObjectTag, _int iObjType)
	: CGameObject(pDevice, pContext, strObjectTag, iObjType)
{
}

CPartObject::CPartObject(const CPartObject& rhs)
	: CGameObject(rhs)
{
}

HRESULT CPartObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPartObject::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		PART_DESC* pPartDesc = (PART_DESC*)pArg;
		m_pOwner = pPartDesc->pOwner;
		m_ePart = pPartDesc->ePart;

		m_pParentTransform = pPartDesc->pParentTransform;
		Safe_AddRef(m_pParentTransform);

		m_pParentModel = pPartDesc->pPartenModel;
		Safe_AddRef(m_pParentModel);

		m_iSocketBoneIndex = pPartDesc->iSocketBoneIndex;
		m_iStoreSocketBoneIndex = pPartDesc->iStoreSocketBoneIndex;

		m_SocketPivotMatrix = pPartDesc->SocketPivotMatrix;

		m_OffSetMatrix = pPartDesc->OffsetMatrix;
		m_strPartModel = pPartDesc->strModel;
	}

	return S_OK;
}

void CPartObject::Tick(_float fTimeDelta)
{
}

void CPartObject::LateTick(_float fTimeDelta)
{
}

HRESULT CPartObject::Render_ShadowDepth()
{
	if (FAILED(m_pShaderCom->Bind_CBuffer("TransformBuffer", &m_WorldMatrix, sizeof(Matrix))))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Push_ShadowVP()))
		return S_OK;

	_uint	iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(m_pModelCom->Render(m_pShaderCom, i, "ShadowPass")))
			return S_OK;
	}
}

void CPartObject::Store_Socket()
{
	if (nullptr == m_pParentModel)
		return;

	m_StoreSocketMatrix = m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex);
	memcpy(&m_StoreSocketPos, m_pParentModel->Get_CombinedMatrix(m_iSocketBoneIndex).m[3], sizeof(Vec3));

	m_IsStored = true;
}

void CPartObject::UnStore_Socket()
{
	m_StoreSocketMatrix = XMMatrixIdentity();
	m_StoreSocketPos = Vec3();

	m_IsStored = false;
}

HRESULT CPartObject::Compute_RenderMatrix(Matrix ChildMatrix)
{
	m_WorldMatrix = ChildMatrix * m_pParentTransform->Get_WorldMatrix();

	return S_OK;
}

CGameObject* CPartObject::Clone(void* pArg)
{
	return nullptr;
}

void CPartObject::Free()
{
	__super::Free();

	Safe_Release(m_pParentTransform);
	Safe_Release(m_pParentModel);
}