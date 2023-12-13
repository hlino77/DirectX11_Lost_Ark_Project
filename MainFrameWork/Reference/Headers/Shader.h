#pragma once

#include "Component.h"
#include "AsTypes.h"
#include "Hasher.h"

BEGIN(Engine)

class CConstantBuffer;
class ENGINE_DLL CShader final : public CComponent
{
	using Super = CComponent;

public:
	typedef struct tagPassDesc
	{
		wstring									strName;
		ID3DX11EffectPass*						pPass = nullptr;
		D3DX11_PASS_SHADER_DESC					passVsDesc;
		D3DX11_EFFECT_SHADER_DESC				effectVsDesc;
		vector<D3D11_SIGNATURE_PARAMETER_DESC>	signatureDescs;

		ID3D11InputLayout* pInputLayout = nullptr;
	}PASSDESC;
	
	typedef struct tagTechniqueDesc
	{
		wstring					strName;
		D3DX11_TECHNIQUE_DESC	desc;
		ID3DX11EffectTechnique* pTechnique = nullptr;
		vector<PASSDESC>		vecPasses;

	}TECHNIQUEDESC;

private:
	CShader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CShader(const CShader& rhs);
	virtual ~CShader() = default;

public:
	virtual HRESULT Initialize_Prototype(const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements);
	virtual HRESULT Initialize(void* pArg);

public:
	/* 셰이더 전역변수에 데이터를 던진다. */
	HRESULT Bind_CBuffer(const string& strCBufferName, const void* pData, _uint iLength = 0);

	HRESULT Bind_RawValue(const _char* pConstantName, const void* pData, _uint iLength);
	HRESULT Bind_Matrix(const _char* pConstantName, const Matrix* pMatrix) const;
	HRESULT Bind_Matrices(const _char* pConstantName, const Matrix* pMatrices, _uint iNumMatrices)const;
	HRESULT Bind_Texture(const _char* pConstantName, ID3D11ShaderResourceView* pSRV) const;
	HRESULT Bind_Textures(const _char* pConstantName, ID3D11ShaderResourceView** ppSRVs, _uint iNumTextures)const;
	HRESULT Dispatch(_uint iTechnique, _uint iPass, _uint iX, _uint iY, _uint iZ);

	HRESULT Get_RawValue(const _char* pConstantName, OUT void* pData, _uint iLength) const;
	HRESULT Get_Matrix(const _char* pConstantName, OUT Matrix* pMatrix) const;
	HRESULT Get_Matrices(const _char* pConstantName, OUT Matrix* pMatrices, _uint iNumMatrices) const;
	HRESULT Get_UAV(const _char* pConstantName, OUT void* pData) const;

	HRESULT SetTechniqeuIndex(_uint iTechniqueIndex);
	HRESULT SetPassIndex(_uint iPassIndex);

public: /* 이 셰이더의 특정 패스로 그린다. */
	HRESULT Begin(_uint iPassIndex, _uint iTechniqueIndex = 0);
	HRESULT Begin();

private:
	ID3DX11EffectConstantBuffer*				GetConstantBuffer(const _char* pName);
	ID3DX11EffectUnorderedAccessViewVariable*	GetUAV(const _char* pName);
	ID3DX11EffectShaderResourceVariable*		GetSRV(const _char* pName);

private:
	ID3DX11Effect*			m_pEffect = nullptr;
	vector<TECHNIQUEDESC>	m_vecTechnique;

	_int					m_iTechniqueIndex = 0;
	_int					m_iPassIndex = 0;

	using CBUFFERS = unordered_map<string, pair<CConstantBuffer*, ComPtr<ID3DX11EffectConstantBuffer>>, djb2Hasher>;
	static CBUFFERS	m_hashConstantBuffers;

public:
	static CShader* Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, const wstring& strShaderFilePath, const D3D11_INPUT_ELEMENT_DESC * pElements, _uint iNumElements);
	virtual CComponent* Clone(CGameObject* pObject, void* pArg);
	virtual void Free() override;
};

END