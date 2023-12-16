#include "..\Public\VIBuffer_RectSSAO.h"
#include "AsTypes.h"

CVIBuffer_RectSSAO::CVIBuffer_RectSSAO(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_RectSSAO::CVIBuffer_RectSSAO(const CVIBuffer_RectSSAO & rhs)
	: CVIBuffer(rhs)
	
{

}

HRESULT CVIBuffer_RectSSAO::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 4;
	m_iStride = sizeof(VTXNORTEX);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXNORTEX*		pVertices = new VTXNORTEX[4];
	ZeroMemory(pVertices, sizeof(VTXNORTEX) * 4);

	//pVertices[0].vPosition = Vec3(-0.5f, 0.5f, 0.f);
	//pVertices[0].vPosition = Vec3(-1.f, -1.f, 0.f);
	pVertices[0].vPosition = Vec3(-0.5f, -0.5f, 0.f);
	pVertices[0].vNormal = Vec3(0.f, 0.f, 0.f);
	pVertices[0].vTexture = Vec2(0.f, 1.f);

	//pVertices[1].vPosition = Vec3(0.5f, 0.5f, 0.f);
	pVertices[1].vPosition = Vec3(-0.5f, 0.5f, 0.f);
	pVertices[1].vNormal = Vec3(1.f, 0.f, 0.f);
	pVertices[1].vTexture = Vec2(0.f, 0.f);

	//pVertices[2].vPosition = Vec3(0.5f, -0.5f, 0.f);
	pVertices[2].vPosition = Vec3(0.5f, 0.5f, 0.f);
	pVertices[2].vNormal = Vec3(2.f, 0.f, 0.f);
	pVertices[2].vTexture = Vec2(1.f, 0.f);

	//pVertices[3].vPosition = Vec3(-0.5f, -0.5f, 0.f);
	pVertices[3].vPosition = Vec3(0.5f, -0.5f, 0.f);
	pVertices[3].vNormal = Vec3(3.f, 0.f, 0.f);
	pVertices[3].vTexture = Vec2(1.f, 1.f);

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(__super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = 2;
	m_iIndexSizeofPrimitive = sizeof(FACEINDICES16);
	m_iNumIndicesofPrimitive = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumPrimitives * m_iIndexSizeofPrimitive;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;


	FACEINDICES16*		pIndices = new FACEINDICES16[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitives);

	pIndices[0]._0 = 0;
	pIndices[0]._1 = 1;
	pIndices[0]._2 = 2;

	pIndices[1]._0 = 0;
	pIndices[1]._1 = 2;
	pIndices[1]._2 = 3;

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_RectSSAO::Initialize(void * pArg)
{
	return S_OK;
}

CVIBuffer_RectSSAO * CVIBuffer_RectSSAO::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_RectSSAO*			pInstance = new CVIBuffer_RectSSAO(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_RectSSAO");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_RectSSAO::Clone(CGameObject* pObject, void * pArg)
{
	CVIBuffer_RectSSAO*			pInstance = new CVIBuffer_RectSSAO(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_RectSSAO");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_RectSSAO::Free()
{
	__super::Free();
}
