#include "..\Public\VIBuffer.h"


CVIBuffer::CVIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{

}

CVIBuffer::CVIBuffer(const CVIBuffer& rhs)
	: CComponent(rhs)
	, m_pVB(rhs.m_pVB)
	, m_pIB(rhs.m_pIB)
	, m_iNumVertices(rhs.m_iNumVertices)
	, m_iStride(rhs.m_iStride)
	, m_iNumPrimitives(rhs.m_iNumPrimitives)
	, m_iIndexSizeofPrimitive(rhs.m_iIndexSizeofPrimitive)
	, m_iNumIndicesofPrimitive(rhs.m_iNumIndicesofPrimitive)
	, m_iNumVertexBuffers(rhs.m_iNumVertexBuffers)
	, m_eIndexFormat(rhs.m_eIndexFormat)
	, m_eTopology(rhs.m_eTopology)
	, m_pVerticesPos(rhs.m_pVerticesPos)
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);

}

HRESULT CVIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CVIBuffer::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
	};

	_uint			iStrides[] = {
		m_iStride
	};

	_uint			iOffsets[] = {
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, iOffsets);

	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	m_pContext->IASetPrimitiveTopology(m_eTopology);

	m_pContext->DrawIndexed(m_iNumPrimitives * m_iNumIndicesofPrimitive, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize)
{
	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = sizeof(VTXINSTANCE);

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		m_iStride,
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0,
		0
	};


	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(m_iNumPrimitives * m_iNumIndicesofPrimitive, iSize, 0, 0, 0);


	return S_OK;
}

HRESULT CVIBuffer::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, _uint iStride)
{

	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = iStride;

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		m_iStride,
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0,
		0
	};


	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, 2, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(m_iNumPrimitives * m_iNumIndicesofPrimitive, iSize, 0, 0, 0);

	return S_OK;
}

HRESULT CVIBuffer::Create_VertexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pVB)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer::Create_IndexBuffer()
{
	if (nullptr == m_pDevice)
		return E_FAIL;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, &m_SubResourceData, &m_pIB)))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pVerticesPos);

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);

}
