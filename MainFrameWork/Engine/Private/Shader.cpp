#include "..\Public\Shader.h"
#include "Utils.h"
#include "ConstantBuffer.h"

CShader::CBUFFERS CShader::m_hashConstantBuffers;

CShader::CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent(pDevice, pContext)
{
}

CShader::CShader(const CShader& rhs)
	: CComponent(rhs)
	, m_pEffect(rhs.m_pEffect)
	, m_vecTechnique(rhs.m_vecTechnique)
{
	for (auto& TechniqueDesc : m_vecTechnique)
	{
		Safe_AddRef(TechniqueDesc.pTechnique);

		for (auto& PassDesc : TechniqueDesc.vecPasses)
		{
			Safe_AddRef(PassDesc.pPass);
			Safe_AddRef(PassDesc.pInputLayout);
		}
	}

	Safe_AddRef(m_pEffect);
}

HRESULT CShader::Initialize_Prototype(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	_uint		iFlag = 0;

#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR/* | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY*/;
#else 
	iFlag = D3DCOMPILE_OPTIMIZATION_LEVEL1 | D3DCOMPILE_PACK_MATRIX_ROW_MAJOR/* | D3DCOMPILE_ENABLE_BACKWARDS_COMPATIBILITY*/;
#endif	

	if (FAILED(D3DX11CompileEffectFromFile(strShaderFilePath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, iFlag, 0, m_pDevice, &m_pEffect, nullptr)))
		return E_FAIL;

	D3DX11_EFFECT_DESC effectDesc;
	m_pEffect->GetDesc(&effectDesc);

	for (UINT t = 0; t < effectDesc.Techniques; t++)
	{
		TECHNIQUEDESC	descTechnique;

		ID3DX11EffectTechnique* pTechnique = m_pEffect->GetTechniqueByIndex(t);
		if (nullptr == pTechnique)
			return E_FAIL;

		D3DX11_TECHNIQUE_DESC		TechniqueDesc;
		ZeroMemory(&TechniqueDesc, sizeof(D3DX11_TECHNIQUE_DESC));

		if (FAILED(pTechnique->GetDesc(&TechniqueDesc)))
			return E_FAIL;

		for (_uint i = 0; i < TechniqueDesc.Passes; ++i)
		{
			PASSDESC				PassDesc;
			ZeroMemory(&PassDesc, sizeof(PASSDESC));

			PassDesc.pPass = pTechnique->GetPassByIndex(i);

			D3DX11_PASS_DESC		PassInfo;

			PassDesc.pPass->GetDesc(&PassInfo);

			CUtils* pUtils = GET_INSTANCE(CUtils);
			PassDesc.strName = pUtils->ToWString(PassInfo.Name);
			RELEASE_INSTANCE(CUtils);

			PassDesc.pPass->GetVertexShaderDesc(&PassDesc.passVsDesc);
			PassDesc.passVsDesc.pShaderVariable->GetShaderDesc(PassDesc.passVsDesc.ShaderIndex, &PassDesc.effectVsDesc);

			if (FAILED(m_pDevice->CreateInputLayout(pElements, iNumElements, PassInfo.pIAInputSignature, PassInfo.IAInputSignatureSize, &PassDesc.pInputLayout)))
				return E_FAIL;

			descTechnique.vecPasses.push_back(PassDesc);
		}

		m_vecTechnique.push_back(descTechnique);
	}

	return S_OK;
}

HRESULT CShader::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CShader::Bind_CBuffer(const string& strCBufferName, const void* pData, _uint iLength)
{
	if (0 == m_hashConstantBuffers.count(strCBufferName))
	{
		if (0 == iLength) return E_FAIL;

		CConstantBuffer* pCBuffer = CConstantBuffer::Create(m_pDevice, m_pContext, iLength);
		ComPtr<ID3DX11EffectConstantBuffer> pEffectCBuffer = GetConstantBuffer(strCBufferName.c_str());

		m_hashConstantBuffers.emplace(strCBufferName, make_pair(pCBuffer, pEffectCBuffer));
	}

	auto& pCBufferPair = m_hashConstantBuffers.find(strCBufferName);
	CConstantBuffer*& pCBuffer = pCBufferPair->second.first;
	pCBuffer->CopyData(pData);

	return pCBufferPair->second.second->SetConstantBuffer(pCBuffer->GetBuffer());
}

HRESULT CShader::Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength)
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->SetRawValue(pData, 0, iLength);
}

HRESULT CShader::Bind_Matrix(const _char* pConstantName, const Matrix* pMatrix) const
{
	/* pConstantName이름에 해당하는 타입을 고려하지않은 전역변수를 컨트롤하는 객체를 얻어온다 .*/
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	return pMatrixVariable->SetMatrix((const _float*)pMatrix);
}

HRESULT CShader::Bind_Matrices(const _char* pConstantName, const Matrix* pMatrices, _uint iNumMatrices) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrix = pVariable->AsMatrix();
	if (nullptr == pMatrix)
		return E_FAIL;

	return pMatrix->SetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::Bind_Texture(const _char* pConstantName, ID3D11ShaderResourceView* pSRV) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResource(pSRV);
}

HRESULT CShader::Bind_Textures(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumTextures) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectShaderResourceVariable* pSRVariable = pVariable->AsShaderResource();
	if (nullptr == pSRVariable)
		return E_FAIL;

	return pSRVariable->SetResourceArray(ppSRVs, 0, iNumTextures);
}

HRESULT CShader::Dispatch(_uint iTechnique, _uint iPass, _uint iX, _uint iY, _uint iZ)
{
	m_pEffect->GetTechniqueByIndex(iTechnique)->GetPassByIndex(iPass)->Apply(0, m_pContext);
	m_pContext->Dispatch(iX, iY, iZ);

	ID3D11ShaderResourceView* null[1] = { 0 };
	m_pContext->CSSetShaderResources(0, 1, null);

	ID3D11UnorderedAccessView* nullUav[1] = { 0 };
	m_pContext->CSSetUnorderedAccessViews(0, 1, nullUav, NULL);

	m_pContext->CSSetShader(NULL, NULL, 0);

	return S_OK;
}

HRESULT CShader::Get_RawValue(const _char* pConstantName, OUT void* pData, _uint iLength) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	return pVariable->GetRawValue(pData, 0, iLength);
}

HRESULT CShader::Get_Matrix(const _char* pConstantName, OUT Matrix* pMatrix) const
{
	/* pConstantName이름에 해당하는 타입을 고려하지않은 전역변수를 컨트롤하는 객체를 얻어온다 .*/
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrixVariable = pVariable->AsMatrix();
	if (nullptr == pMatrixVariable)
		return E_FAIL;

	if (FAILED(pMatrixVariable->GetMatrix((_float*)pMatrix)))
		return E_FAIL;

	return S_OK;
}

HRESULT CShader::Get_Matrices(const _char* pConstantName, OUT Matrix* pMatrices, _uint iNumMatrices) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectMatrixVariable* pMatrix = pVariable->AsMatrix();
	if (nullptr == pMatrix)
		return E_FAIL;

	return pMatrix->GetMatrixArray((_float*)pMatrices, 0, iNumMatrices);
}

HRESULT CShader::Get_UAV(const _char* pConstantName, OUT void* pData) const
{
	ID3DX11EffectVariable* pVariable = m_pEffect->GetVariableByName(pConstantName);
	if (nullptr == pVariable)
		return E_FAIL;

	ID3DX11EffectUnorderedAccessViewVariable* pUAV = pVariable->AsUnorderedAccessView();
	if (nullptr == pUAV)
		return E_FAIL;

	return pUAV->SetUnorderedAccessView((ID3D11UnorderedAccessView*)pData);
}

HRESULT CShader::SetTechniqeuIndex(_uint iTechniqueIndex)
{
	if (iTechniqueIndex >= m_vecTechnique.size())
		return E_FAIL;

	m_iTechniqueIndex = iTechniqueIndex;

	return S_OK;
}

HRESULT CShader::SetPassIndex(_uint iPassIndex)
{
	if (iPassIndex >= m_vecTechnique[iPassIndex].vecPasses.size())
		return E_FAIL;

	m_iPassIndex = iPassIndex;

	return S_OK;
}

HRESULT CShader::Begin(_uint iPassIndex, _uint iTechniqueIndex)
{
	if(iTechniqueIndex >= m_vecTechnique.size())
		return E_FAIL;

	if (iPassIndex >= m_vecTechnique[iTechniqueIndex].vecPasses.size())
		return E_FAIL;

	/* 이러한 정보를 가진 정점을 그리낟. */
	/* 이러한 정보를 가진  : 이미 셰이더에서 받을 수 있음을 검증했다. */
	m_pContext->IASetInputLayout(m_vecTechnique[iTechniqueIndex].vecPasses[iPassIndex].pInputLayout);

	/* 해당 ID3DXEffectPass로 그리낟. */
	/* Apply를 호출하기전에 모든 변수를 셰이더로 전달하는 역활을 마무리해야한다. */
	m_vecTechnique[iTechniqueIndex].vecPasses[iPassIndex].pPass->Apply(0, m_pContext);

	return S_OK;
}

HRESULT CShader::Begin()
{
	if (m_iTechniqueIndex >= m_vecTechnique.size())
		return E_FAIL;

	if (m_iPassIndex >= m_vecTechnique[m_iTechniqueIndex].vecPasses.size())
		return E_FAIL;

	/* 이러한 정보를 가진 정점을 그리낟. */
	/* 이러한 정보를 가진  : 이미 셰이더에서 받을 수 있음을 검증했다. */
	m_pContext->IASetInputLayout(m_vecTechnique[m_iTechniqueIndex].vecPasses[m_iPassIndex].pInputLayout);

	/* 해당 ID3DXEffectPass로 그리낟. */
	/* Apply를 호출하기전에 모든 변수를 셰이더로 전달하는 역활을 마무리해야한다. */
	m_vecTechnique[m_iTechniqueIndex].vecPasses[m_iPassIndex].pPass->Apply(0, m_pContext);

	return S_OK;
}

ID3DX11EffectConstantBuffer* CShader::GetConstantBuffer(const _char* szName)
{
	return m_pEffect->GetConstantBufferByName(szName);
}

ID3DX11EffectUnorderedAccessViewVariable* CShader::GetUAV(const _char* szName)
{
	return m_pEffect->GetVariableByName(szName)->AsUnorderedAccessView();
}

ID3DX11EffectShaderResourceVariable* CShader::GetSRV(const _char* szName)
{
	return m_pEffect->GetVariableByName(szName)->AsShaderResource();
}

CShader* CShader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC* pElements, _uint iNumElements)
{
	CShader* pInstance = new CShader(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(strShaderFilePath, pElements, iNumElements)))
	{
		MSG_BOX("Failed To Created : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CShader::Clone(CGameObject* pObject, void* pArg)
{
	CShader* pInstance = new CShader(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CShader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CShader::Free()
{
	Super::Free();

	for (auto& TechniqueDesc : m_vecTechnique)
	{
		Safe_Release(TechniqueDesc.pTechnique);

		for (auto& PassDesc : TechniqueDesc.vecPasses)
		{
			Safe_Release(PassDesc.pInputLayout);
			Safe_Release(PassDesc.pPass);
		}

		TechniqueDesc.vecPasses.clear();
	}

	m_vecTechnique.clear();

	Safe_Release(m_pEffect);
}