#include "..\Public\VIBuffer_Trail.h"

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
	m_iNumVertices = 100000;
	m_iStride = sizeof(VTXTRAIL);

	ZeroMemory(&m_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	m_BufferDesc.ByteWidth = m_iNumVertices * m_iStride;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC/*D3D11_USAGE_DEFAULT*/;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE/*0*/;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = m_iStride;

	VTXTRAIL* pVertices = new VTXTRAIL[m_iNumVertices];

	for (_uint i = 0; i < 100000; i += 2)
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

void CVIBuffer_Trail::Update_TrailBuffer(Matrix& TransformMatrix)
{
	vector<VTXTRAIL> Vertices;
	Vertices.reserve(m_iNumVertices);

	TransformMatrix.Right(XMVector4Normalize(TransformMatrix.Right()));
	TransformMatrix.Up(XMVector4Normalize(TransformMatrix.Up()));
	TransformMatrix.Backward(XMVector4Normalize(TransformMatrix.Backward()));

	Vec4 vWorldHigh = XMVector3TransformCoord(XMLoadFloat4(&m_vHighPosition), TransformMatrix);
	Vec4 vWorldLow = XMVector3TransformCoord(XMLoadFloat4(&m_vLowPosition), TransformMatrix);

	if (m_TrailVertices.size() < m_iVtxCount - 2)
	{
		VTXTRAIL Vertex = {};
		XMStoreFloat3(&Vertex.vPosition, vWorldHigh);
		m_TrailVertices.push_back(Vertex);

		XMStoreFloat3(&Vertex.vPosition, vWorldLow);
		m_TrailVertices.push_back(Vertex);
	}

	while (m_iVtxCount - 2 < m_TrailVertices.size())
		m_TrailVertices.pop_front();

	VTXTRAIL Vertex = {};
	XMStoreFloat3(&Vertex.vPosition, vWorldHigh);
	m_TrailVertices.push_back(Vertex);

	XMStoreFloat3(&Vertex.vPosition, vWorldLow);
	m_TrailVertices.push_back(Vertex);

	for (_uint i = 0; i < m_TrailVertices.size(); i++)
	{
		_float _fVtxCount = _float(m_TrailVertices.size());

		_float fUV = ((_float)i) / (_fVtxCount);

		if (i % 2 == 0)
			m_TrailVertices[i].vTexture = { fUV, 0.f };
		else
			m_TrailVertices[i].vTexture = { fUV, 1.f };

		m_TrailVertices[i].fAlpha = fUV;

		Vertices.push_back(m_TrailVertices[i]);
	}

	vector<VTXTRAIL> CatmallomVertices;
	CatmallomVertices.reserve((m_iVtxCount - 2) * 5);
	for (_uint i = 0; i < Vertices.size(); i += 2)
	{
		if (i >= Vertices.size() - 4)
		{
			CatmallomVertices.push_back(Vertices[i]);
			CatmallomVertices.push_back(Vertices[i + 1]);
			continue;
		}

		VTXTRAIL vHigh0, vHigh1, vHigh2, vHigh3;
		VTXTRAIL vLow0, vLow1, vLow2, vLow3;

		if (i == 0)
		{
			vHigh0 = Vertices[i];
			vLow0 = Vertices[i + 1];
		}
		else
		{
			vHigh0 = Vertices[i - 2];
			vLow0 = Vertices[i - 1];
		}

		vHigh1 = Vertices[i];
		vHigh2 = Vertices[i + 2];
		vHigh3 = Vertices[i + 4];

		vLow1 = Vertices[i + 1];
		vLow2 = Vertices[i + 3];
		vLow3 = Vertices[i + 5];

		_float fInvCount = 1.f / m_iCatmullRomCount;
		for (_uint j = 0; j < m_iCatmullRomCount; ++j)
		{
			VTXTRAIL VertexHigh = {};
			XMStoreFloat3(&VertexHigh.vPosition, XMVectorCatmullRom(
				XMLoadFloat3(&vHigh0.vPosition)
				, XMLoadFloat3(&vHigh1.vPosition)
				, XMLoadFloat3(&vHigh2.vPosition)
				, XMLoadFloat3(&vHigh3.vPosition)
				, j * fInvCount));

			XMStoreFloat2(&VertexHigh.vTexture, XMVectorCatmullRom(
				XMLoadFloat2(&vHigh0.vTexture)
				, XMLoadFloat2(&vHigh1.vTexture)
				, XMLoadFloat2(&vHigh2.vTexture)
				, XMLoadFloat2(&vHigh3.vTexture)
				, j * fInvCount));

			XMStoreFloat(&VertexHigh.fAlpha, XMVectorCatmullRom(
				XMLoadFloat(&vHigh0.fAlpha)
				, XMLoadFloat(&vHigh1.fAlpha)
				, XMLoadFloat(&vHigh2.fAlpha)
				, XMLoadFloat(&vHigh3.fAlpha)
				, j * fInvCount));

			CatmallomVertices.push_back(VertexHigh);

			VTXTRAIL VertexLow = {};
			XMStoreFloat3(&VertexLow.vPosition, XMVectorCatmullRom(
				XMLoadFloat3(&vLow0.vPosition)
				, XMLoadFloat3(&vLow1.vPosition)
				, XMLoadFloat3(&vLow2.vPosition)
				, XMLoadFloat3(&vLow3.vPosition)
				, j * fInvCount));

			XMStoreFloat2(&VertexLow.vTexture, XMVectorCatmullRom(
				XMLoadFloat2(&vLow0.vTexture)
				, XMLoadFloat2(&vLow1.vTexture)
				, XMLoadFloat2(&vLow2.vTexture)
				, XMLoadFloat2(&vLow3.vTexture)
				, j * fInvCount));

			XMStoreFloat(&VertexHigh.fAlpha, XMVectorCatmullRom(
				XMLoadFloat(&vHigh0.fAlpha)
				, XMLoadFloat(&vHigh1.fAlpha)
				, XMLoadFloat(&vHigh2.fAlpha)
				, XMLoadFloat(&vHigh3.fAlpha)
				, j * fInvCount));

			CatmallomVertices.push_back(VertexLow);
		}
	}

	m_iNumPrimitives = CatmallomVertices.size() - 2;
	//m_iNumPrimitives = Vertices.size() - 2;

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	ZeroMemory(&mappedResource, sizeof(D3D11_MAPPED_SUBRESOURCE));
	m_pContext->Map(m_pVB, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	memcpy(mappedResource.pData, CatmallomVertices.data(), sizeof(VTXTRAIL) * CatmallomVertices.size());
	//memcpy(mappedResource.pData, Vertices.data(), sizeof(VTXTRAIL) * Vertices.size());
	m_pContext->Unmap(m_pVB, 0);
}

HRESULT CVIBuffer_Trail::Render()
{
	if (FAILED(Super::Render()))
		return E_FAIL;

	return S_OK;
}

void CVIBuffer_Trail::SetUp_Position(Vec4 vHighPosition, Vec4 vLowPosition)
{
	XMStoreFloat4(&m_vHighPosition, vHighPosition);
	XMStoreFloat4(&m_vLowPosition, vLowPosition);
}

void CVIBuffer_Trail::Start_Trail(Matrix TransformMatrix)
{
	m_TrailVertices.clear();
	for (_uint i = 0; i < m_iNumVertices; i += 2)
	{
		VTXTRAIL High = {};
		XMStoreFloat3(&High.vPosition, XMVector3TransformCoord(XMLoadFloat4(&m_vHighPosition), TransformMatrix));
		VTXTRAIL Low = {};
		XMStoreFloat3(&Low.vPosition, XMVector3TransformCoord(XMLoadFloat4(&m_vLowPosition), TransformMatrix));

		m_TrailVertices.push_back(High);
		m_TrailVertices.push_back(Low);
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

	return pInstance;
}

void CVIBuffer_Trail::Free()
{
	Super::Free();
}
