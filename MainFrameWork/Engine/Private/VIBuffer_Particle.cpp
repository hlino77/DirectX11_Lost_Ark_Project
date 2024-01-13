#include "..\Public\VIBuffer_Particle.h"
#include "Shader.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CVIBuffer(pDevice, pContext)
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& rhs)
	: CVIBuffer(rhs)
	, m_pDrawVB(rhs.m_pDrawVB)
	, m_pStreamOutVB(rhs.m_pStreamOutVB)
{
	Safe_AddRef(m_pDrawVB);
	Safe_AddRef(m_pStreamOutVB);
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
	/*m_iStride = sizeof(VTXPARTICLE);
	m_iNumVertices = 1;
	m_iNumVBs = 1;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	VTXPARTICLE*		pVertices = new VTXPARTICLE;
	ZeroMemory(pVertices, sizeof(VTXPARTICLE));

	pVertices->vPosition = _float3(0.f, 0.f, 0.f);
	pVertices->vSize = _float2(1.f, 1.f);
	pVertices->fAge = 0.f;
	pVertices->iType = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	m_BufferDesc.ByteWidth = sizeof(VTXPARTICLE) * m_fMaxParticles;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, 0, &m_pDrawVB)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, 0, &m_pStreamOutVB)))
		return E_FAIL;
#pragma endregion*/

	return S_OK;
}

HRESULT CVIBuffer_Particle::Initialize(void * pArg)
{
	m_iStride = sizeof(VTXPARTICLE);
	m_iNumVertices = 1;
	m_iNumVertexBuffers = 1;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEX_BUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);

	m_BufferDesc.ByteWidth = m_iStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;
	m_BufferDesc.StructureByteStride = 0;

	VTXPARTICLE* pVertices = new VTXPARTICLE;
	ZeroMemory(pVertices, sizeof(VTXPARTICLE));

	pVertices->vPosition = XMFLOAT3(0.f, 0.f, 0.f);
	pVertices->vSize = XMFLOAT2(1.f, 1.f);
	pVertices->fAge = 0.f;
	pVertices->iType = 0;

	ZeroMemory(&m_SubResourceData, sizeof m_SubResourceData);
	m_SubResourceData.pSysMem = pVertices;

	if (FAILED(Super::Create_VertexBuffer()))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

	m_BufferDesc.ByteWidth = sizeof(VTXPARTICLE) * m_fMaxParticles;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, 0, &m_pDrawVB)))
		return E_FAIL;
	if (FAILED(m_pDevice->CreateBuffer(&m_BufferDesc, 0, &m_pStreamOutVB)))
		return E_FAIL;
#pragma endregion

	return S_OK;
}

HRESULT CVIBuffer_Particle::Render(string strPathName)
{
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pDrawVB
	};

	_uint			iStrides[] = {
		m_iStride,
	};

	_uint			iOffsets[] = {
		0,
	};

	// On the first pass, use the initialization VB.  Otherwise, use
	// the VB that contains the current particle list.
	if (m_IsFirstRun)
		m_pContext->IASetVertexBuffers(0, 1, &pVertexBuffers[0], iStrides, iOffsets);
	else
		m_pContext->IASetVertexBuffers(0, 1, &pVertexBuffers[1], iStrides, iOffsets);

	//
	// Draw the current particle list using stream-out only to update them.  
	// The updated vertices are streamed-out to the target VB. 
	//
	m_pContext->SOSetTargets(1, &m_pStreamOutVB, iOffsets);

	if (FAILED(m_pOwner->Get_ShaderCom()->Begin(0/*"Default"*/, 0)))
		return E_FAIL;

	if (m_IsFirstRun)
	{
		m_pContext->Draw(1, 0);
		m_IsFirstRun = false;
	}
	else
	{
		m_pContext->DrawAuto();
	}

	// done streaming-out--unbind the vertex buffer
	ID3D11Buffer* bufferArray[1] = { 0 };
	m_pContext->SOSetTargets(1, bufferArray, iOffsets);

	// ping-pong the vertex buffers
	std::swap(m_pDrawVB, m_pStreamOutVB);

	//
	// Draw the updated particle system we just streamed-out. 
	//
	m_pContext->IASetVertexBuffers(0, 1, &m_pDrawVB, iStrides, iOffsets);

	if (FAILED(m_pOwner->Get_ShaderCom()->Begin(strPathName, 1)))
		return E_FAIL;

	m_pContext->DrawAuto();

	return S_OK;
}

void CVIBuffer_Particle::Reset()
{
	m_IsFirstRun = true;
}

void CVIBuffer_Particle::SetEyePos(const Vec3& vEyePosW)
{
	m_vEyePosW = vEyePosW;
}

void CVIBuffer_Particle::SetEmitPos(const Vec3& vEmitPosW)
{
	m_vEmitPosW = vEmitPosW;
}

void CVIBuffer_Particle::SetEmitDir(const Vec3& vEmitDirW)
{
	m_vEmitDirW = vEmitDirW;
}

CVIBuffer_Particle * CVIBuffer_Particle::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CVIBuffer_Particle*	pInstance = new CVIBuffer_Particle(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CVIBuffer_Particle::Clone(CGameObject* pGameObject, void * pArg)
{
	CVIBuffer_Particle*	pInstance = new CVIBuffer_Particle(*this);
	pInstance->m_pOwner = pGameObject;

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Particle");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Particle::Free()
{
	Safe_Release(m_pDrawVB);
	Safe_Release(m_pStreamOutVB);

	Super::Free();
}