#include "..\Public\VIBuffer_Trail.h"
#include "AsUtils.h"


CVIBuffer_Trail::CVIBuffer_Trail(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Trail::CVIBuffer_Trail(const CVIBuffer_Trail & rhs)
	: CVIBuffer(rhs)
{

}

HRESULT CVIBuffer_Trail::Initialize_Prototype()
{
#pragma region VERTEXBUFFER
	m_iNumVertexBuffers = 1;
	m_iNumVertices = 10000;
	m_iStride = sizeof(VTXTRAIL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC/*D3D11_USAGE_DEFAULT*/;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE/*0*/;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXTRAIL* pVertices = new VTXTRAIL[m_iNumVertices];

	for (_uint i = 0; i < m_iNumVertices; i += 2)
	{
		pVertices[i].vPosition = Vec3(0.f, 0.f, 0.f);
		pVertices[i].vTexture = Vec2(0.f, 0.f);

		pVertices[i + 1].vPosition = Vec3(0.f, 0.f, 0.f);
		pVertices[i + 1].vTexture = Vec2(0.f, 0.f);
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEXBUFFER
	m_iNumPrimitives = m_iNumVertices - 2;
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

	FACEINDICES16* pIndices = new FACEINDICES16[m_iNumPrimitives];
	ZeroMemory(pIndices, sizeof(FACEINDICES16) * m_iNumPrimitives);

	for (_uint i = 0; i < m_iNumPrimitives; i += 2)
	{
		pIndices[i]._0 = i + 3;
		pIndices[i]._1 = i + 1;
		pIndices[i]._2 = i;

		pIndices[i + 1]._0 = i + 2;
		pIndices[i + 1]._1 = i + 3;
		pIndices[i + 1]._2 = i;
	}

	ZeroMemory(&m_SubResourceData, sizeof(D3D11_SUBRESOURCE_DATA));
	m_SubResourceData.pSysMem = pIndices;

	if (FAILED(Super::Create_IndexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Trail::Initialize(void * pArg)
{

	return S_OK;
}

void CVIBuffer_Trail::Update_TrailVertices(Matrix& TransformMatrix)
{
	Vec3 vWorldHigh = XMVector3TransformCoord(m_vHighPosition, TransformMatrix);


	Vec3 vWorldLow = XMVector3TransformCoord(m_vLowPosition, TransformMatrix);


	if (m_TrailVertices.size() < 4)
	{
		VTXTRAIL tVertexHigh;
		tVertexHigh.vPosition = vWorldHigh;

		VTXTRAIL tVertexLow;
		tVertexLow.vPosition = vWorldLow;

		m_TrailVertices.push_front(tVertexLow);
		m_TrailVertices.push_front(tVertexHigh);
		
		m_pOwner->Set_Render(false);
	}
	else
	{
		_uint iMaxIndex = m_TrailVertices.size() - 1;

		Vec3 vHigh0 = vWorldHigh;
		Vec3 vHigh1 = m_TrailVertices[0].vPosition;
		Vec3 vHigh2 = m_TrailVertices[2].vPosition;

		_uint iHighIndex = 2 + m_iCatmullRomCount;
		if (iMaxIndex - 1 < 2 + m_iCatmullRomCount)
		{
			iHighIndex = iMaxIndex - 1;
		}
		Vec3 vHigh3 = m_TrailVertices[iHighIndex].vPosition;


		Vec3 vLow0 = vWorldLow;
		Vec3 vLow1 = m_TrailVertices[1].vPosition;
		Vec3 vLow2 = m_TrailVertices[3].vPosition;

		_uint iLowIndex = 3 + m_iCatmullRomCount;
		if (iMaxIndex < 2 + m_iCatmullRomCount)
		{
			iLowIndex = iMaxIndex;
		}
		Vec3 vLow3 = m_TrailVertices[iLowIndex].vPosition;

		m_TrailVertices.pop_front();
		m_TrailVertices.pop_front();

		_float fRatio = 1.0f / m_iCatmullRomCount;
		for (_uint i = 1; i <= m_iCatmullRomCount; ++i)
		{
			VTXTRAIL tVertexHigh;
			tVertexHigh.vPosition = XMVectorCatmullRom(vHigh3, vHigh2, vHigh1, vHigh0, fRatio * i);

			VTXTRAIL tVertexLow;
			tVertexLow.vPosition = XMVectorCatmullRom(vLow3, vLow2, vLow1, vLow0, fRatio * i);

			m_TrailVertices.push_front(tVertexLow);
			m_TrailVertices.push_front(tVertexHigh);
		}

		{
			VTXTRAIL tVertexHigh;
			tVertexHigh.vPosition = vWorldHigh;

			VTXTRAIL tVertexLow;
			tVertexLow.vPosition = vWorldLow;

			m_TrailVertices.push_front(tVertexLow);
			m_TrailVertices.push_front(tVertexHigh);
		}


		_int iPopSize = m_TrailVertices.size() - m_iVtxCount;
		if (iPopSize > 0)
		{
			for (_uint i = 0; i < iPopSize; ++i)
			{
				m_TrailVertices.pop_back();
			}
		}

		m_pOwner->Set_Render(true);
	}
}

void CVIBuffer_Trail::Update_TrailBuffer()
{
	_uint iSize = m_TrailVertices.size();
	_float fRatio = 1.0f / (iSize - 1);
	for (_uint i = 0; i < iSize; ++i)
	{
		_float fTexY = i % 2;

		m_TrailVertices[i].vTexture = XMVectorLerp(Vec2(1.0f, fTexY), Vec2(0.0f, fTexY), i * fRatio);
		m_TrailVertices[i].fAlpha = CAsUtils::Lerpf(1.0f, 0.0f, i * fRatio);

		m_BufferData[i] = m_TrailVertices[i];
	}

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, m_BufferData.data(), sizeof(VTXTRAIL) * m_TrailVertices.size());
	m_pContext->Unmap(m_pVB, 0);

}

HRESULT CVIBuffer_Trail::Render()
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

	m_pContext->DrawIndexed((m_TrailVertices.size() - 2) * m_iNumIndicesofPrimitive, 0, 0);

	return S_OK;
}

void CVIBuffer_Trail::SetUp_Position(Vec3 vHighPosition, Vec3 vLowPosition)
{
	m_vHighPosition = vHighPosition;
	m_vLowPosition = vLowPosition;
}

void CVIBuffer_Trail::Start_Trail(Matrix TransformMatrix)
{
	m_TrailVertices.clear();
	{
		VTXTRAIL tVertexHigh;
		tVertexHigh.vPosition = XMVector3TransformCoord(m_vHighPosition, TransformMatrix);;

		VTXTRAIL tVertexLow;
		tVertexLow.vPosition = XMVector3TransformCoord(m_vLowPosition, TransformMatrix);;

		m_TrailVertices.push_front(tVertexLow);
		m_TrailVertices.push_front(tVertexHigh);
	}
}

void CVIBuffer_Trail::Stop_Trail()
{
	m_TrailVertices.clear();
}

CVIBuffer_Trail * CVIBuffer_Trail::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Trail*			pInstance = new CVIBuffer_Trail(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Trail::Clone(CGameObject* pObject, void * pArg)
{
	CVIBuffer_Trail*			pInstance = new CVIBuffer_Trail(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Trail");
		Safe_Release(pInstance);
	}

	pInstance->m_pOwner = pObject;

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	Super::Free();
}
