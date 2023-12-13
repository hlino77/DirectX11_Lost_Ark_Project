#include "..\Public\PartObject.h"
#include "Transform.h"
#include "Model.h"

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
		Safe_AddRef(m_pParentTransform);

		m_iSocketBoneIndex = pPartDesc->iSocketBoneIndex;

		m_SocketPivotMatrix = pPartDesc->SocketPivotMatrix;
	}


	return S_OK;
}

void CPartObject::Tick(_float fTimeDelta)
{
}

void CPartObject::LateTick(_float fTimeDelta)
{
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