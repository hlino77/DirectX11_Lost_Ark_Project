#include "..\Public\Mesh.h"
#include "Model.h"
#include "AsFileUtils.h"
#include "AsUtils.h"

CMesh::CMesh(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CMesh::CMesh(const CMesh & rhs)
	: CVIBuffer(rhs)	
	, m_iMaterialIndex(rhs.m_iMaterialIndex)
	, m_szName(rhs.m_szName)
	, m_szMaterialName(rhs.m_szMaterialName)
	, m_iBoneIndex(rhs.m_iBoneIndex)
	, m_pVertices(rhs.m_pVertices)
	, m_pIndices(rhs.m_pIndices)
{
	//strcpy_s(m_szName, rhs.m_szName);
}

HRESULT CMesh::Initialize_Prototype(CModel* pModel, Matrix PivotMatrix)
{

	return S_OK;
}

HRESULT CMesh::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CMesh::LoadData_FromMeshFile(CModel::TYPE eModelType, CAsFileUtils* pFileUtils, Matrix PivotMatrix, _bool bColMesh)
{
	m_szName = CAsUtils::ToWString(pFileUtils->Read<string>());
	m_iBoneIndex = pFileUtils->Read<int32>();

	// Material
	m_szMaterialName = CAsUtils::ToWString(pFileUtils->Read<string>());
	m_iMaterialIndex = pFileUtils->Read<int32>();

	//VertexData
	{
		const uint32 iVTXCount = pFileUtils->Read<uint32>();

		m_iNumVertexBuffers = 1;
		m_iNumVertices = iVTXCount;
		m_iStride = sizeof(VTXANIMMODEL);

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iStride;


		if (bColMesh)
		{
			m_pVertices = new VTXANIMMODEL[iVTXCount];
			ZeroMemory(m_pVertices, sizeof(VTXANIMMODEL) * iVTXCount);


			void* pData = m_pVertices;
			pFileUtils->Read(&pData, sizeof(VTXANIMMODEL) * iVTXCount);

			if (eModelType == CModel::TYPE::TYPE_NONANIM)
				Ready_Vertices(m_pVertices, PivotMatrix);

			ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			m_SubResourceData.pSysMem = m_pVertices;

			if (FAILED(__super::Create_VertexBuffer()))
				return E_FAIL;
		}
		else
		{
			VTXANIMMODEL* pVertices = new VTXANIMMODEL[iVTXCount];
			ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iVTXCount);


			void* pData = pVertices;
			pFileUtils->Read(&pData, sizeof(VTXANIMMODEL) * iVTXCount);

			if (eModelType == CModel::TYPE::TYPE_NONANIM)
				Ready_Vertices(pVertices, PivotMatrix);

			ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			m_SubResourceData.pSysMem = pVertices;

			if (FAILED(__super::Create_VertexBuffer()))
				return E_FAIL;

			Safe_Delete_Array(pVertices);
		}
	}

	//IndexData
	{
		const uint32 iIDXCount = pFileUtils->Read<uint32>();

		m_iNumPrimitives = iIDXCount / 3;
		m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
		m_iNumIndicesofPrimitive = 3;
		m_eIndexFormat = DXGI_FORMAT_R32_UINT;
		m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = 0;


		if (bColMesh)
		{
			m_pIndices = new FACEINDICES32[m_iNumPrimitives];
			ZeroMemory(m_pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

			void* pData = m_pIndices;
			pFileUtils->Read(&pData, sizeof(FACEINDICES32) * m_iNumPrimitives);

			ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			m_SubResourceData.pSysMem = m_pIndices;

			if (FAILED(__super::Create_IndexBuffer()))
				return E_FAIL;
		}
		else
		{
			FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
			ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

			void* pData = pIndices;
			pFileUtils->Read(&pData, sizeof(FACEINDICES32) * m_iNumPrimitives);

			ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
			m_SubResourceData.pSysMem = pIndices;

			if (FAILED(__super::Create_IndexBuffer()))
				return E_FAIL;

			Safe_Delete_Array(pIndices);
		}
	}

	return S_OK;
}

HRESULT CMesh::LoadData_FromConverter(CModel::TYPE eModelType, shared_ptr<asMesh> pMesh, Matrix PivotMatrix)
{
	m_szName = CAsUtils::ToWString(pMesh->name);
	m_iBoneIndex = pMesh->boneIndex;

	// Material
	m_szMaterialName = CAsUtils::ToWString(pMesh->materialName);
	m_iMaterialIndex = pMesh->materialIndex;

	//VertexData
	{
		const uint32 iVTXCount = pMesh->vertices.size();

		m_iNumVertexBuffers = 1;
		m_iNumVertices = iVTXCount;
		m_iStride = sizeof(VTXANIMMODEL);

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = m_iStride;


		VTXANIMMODEL* pVertices = new VTXANIMMODEL[iVTXCount];
		ZeroMemory(pVertices, sizeof(VTXANIMMODEL) * iVTXCount);


		memcpy(pVertices, pMesh->vertices.data(), sizeof(VTXANIMMODEL) * iVTXCount);

		
		if (eModelType == CModel::TYPE::TYPE_NONANIM)
			Ready_Vertices(pVertices, PivotMatrix);

		ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pVertices;

		if (FAILED(__super::Create_VertexBuffer()))
			return E_FAIL;

		Safe_Delete_Array(pVertices);
	}

	//IndexData
	{
		const uint32 iIDXCount = pMesh->indices.size();

		m_iNumPrimitives = iIDXCount / 3;
		m_iIndexSizeofPrimitive = sizeof(FACEINDICES32);
		m_iNumIndicesofPrimitive = 3;
		m_eIndexFormat = DXGI_FORMAT_R32_UINT;
		m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

		ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
		m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
		m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		m_BufferDesc.CPUAccessFlags = 0;
		m_BufferDesc.MiscFlags = 0;
		m_BufferDesc.StructureByteStride = 0;


		FACEINDICES32* pIndices = new FACEINDICES32[m_iNumPrimitives];
		ZeroMemory(pIndices, sizeof(FACEINDICES32) * m_iNumPrimitives);

		memcpy(pIndices, pMesh->indices.data(), sizeof(FACEINDICES32) * m_iNumPrimitives);

		ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
		m_SubResourceData.pSysMem = pIndices;

		if (FAILED(__super::Create_IndexBuffer()))
			return E_FAIL;

		Safe_Delete_Array(pIndices);
	}

	return S_OK;
}



HRESULT CMesh::Ready_Vertices(VTXANIMMODEL* pVertices, Matrix PivotMatrix)
{
	for (_uint i = 0; i < m_iNumVertices; ++i)
	{
		{
			_float fY = *&pVertices[i].vPosition.y;
			*&pVertices[i].vPosition.y = -*&pVertices[i].vPosition.z;
			*&pVertices[i].vPosition.z = fY;
		}
		
		{
			_float fY = *&pVertices[i].vNormal.y;
			*&pVertices[i].vNormal.y = -*&pVertices[i].vNormal.z;
			*&pVertices[i].vNormal.z = fY;
		}
		
		{
			_float fY = *&pVertices[i].vTangent.y;
			*&pVertices[i].vTangent.y = -*&pVertices[i].vTangent.z;
			*&pVertices[i].vTangent.z = fY;
		}
	
		XMStoreFloat3(&pVertices[i].vPosition, XMVector3TransformCoord(XMLoadFloat3(&pVertices[i].vPosition), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vNormal, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vNormal), PivotMatrix));
		XMStoreFloat3(&pVertices[i].vTangent, XMVector3TransformNormal(XMLoadFloat3(&pVertices[i].vTangent), PivotMatrix));
	}

	return S_OK;
}


CMesh * CMesh::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, CModel::TYPE eModelType, CModel* pModel, Matrix PivotMatrix)
{
	CMesh*			pInstance = new CMesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pModel, PivotMatrix)))
	{
		MSG_BOX("Failed To Created : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CMesh::Free()
{
	__super::Free();
}

CComponent* CMesh::Clone(CGameObject* pGameObject, void* pArg)
{
	CMesh* pInstance = new CMesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}
