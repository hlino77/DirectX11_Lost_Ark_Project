#include "VIBuffer_Point.h"

CVIBuffer_Point::CVIBuffer_Point(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Point::CVIBuffer_Point(const CVIBuffer_Point & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Point::Initialize_Prototype()
{
#pragma region INDEX_BUFFER

	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

	/* 정점버퍼와 인덱스 버퍼를 만드낟. */
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = 2;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	_ushort*		pIndices = new _ushort;
	*pIndices = 0;


	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(__super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete(pIndices);
#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Point::Initialize(void * pArg)
{

	return S_OK;
}

HRESULT CVIBuffer_Point::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize)
{
	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = sizeof(VTXINSTANCE);

	ID3D11Buffer* pVertexBuffers[] = {
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0
	};


	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, 1, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(1, iSize, 0, 0, 0);


	return S_OK;
}

HRESULT CVIBuffer_Point::Render_Instance(ID3D11Buffer* pInstanceBuffer, _uint iSize, _uint iStride)
{
	if (nullptr == m_pContext)
		return E_FAIL;


	_uint iStrideInstance = iStride;

	ID3D11Buffer* pVertexBuffers[] = {
		pInstanceBuffer
	};

	_uint			iStrides[] = {
		iStrideInstance
	};

	_uint			iOffsets[] = {
		0
	};


	/* 버텍스 버퍼들을 할당한다. */
	/* 그리기용 정점버퍼 + 상태변환용 정점버퍼 */
	m_pContext->IASetVertexBuffers(0, 1, pVertexBuffers, iStrides, iOffsets);

	/* 인덱스 버퍼를 할당한다. */
	/* 그리고자 하는 인스턴스의 갯수만큼 확대되어있는 인덱스 버퍼. */
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);

	/* 해당 정점들을 어떤 방식으로 그릴꺼야. */
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	/* 인덱스가 가르키는 정점을 활용하여 그린다. */
	m_pContext->DrawIndexedInstanced(1, iSize, 0, 0, 0);


	return S_OK;
}


CVIBuffer_Point * CVIBuffer_Point::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Point::Clone(CGameObject* pObject, void* pArg)
{
	CVIBuffer_Point*	pInstance = new CVIBuffer_Point(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Point");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CVIBuffer_Point::Free()
{
	__super::Free();


}
