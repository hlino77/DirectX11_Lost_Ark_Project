#pragma once

/* 렌더타겟들을 생성해서 모아둔다. */
#include "Base.h"
#include "Hasher.h"

BEGIN(Engine)

class CShader;
class CRenderTarget;
class ENGINE_DLL CTarget_Manager final : public CBase
{
	DECLARE_SINGLETON(CTarget_Manager)
private:
	CTarget_Manager();
	virtual ~CTarget_Manager() = default;

public:
	HRESULT Add_RenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const wstring& strTargetTag, _float fSizeX, _float fSizeY, DXGI_FORMAT ePixelFormat, const Vec4& vColor);
	HRESULT Add_MRT(const wstring& strMRTTag, const wstring& strTargetTag);
	HRESULT Bind_SRV(CShader* pShader, const wstring& strTargetTag, const _char* pConstantName);
	HRESULT Make_SRVTexture(const wstring& szPath, const wstring& strTargetTag);
	HRESULT Make_SRVWinTexture(const wstring & szPath, const wstring & strTargetTag);
	HRESULT	Copy_SRV(const wstring& strTargetTag, ID3D11ShaderResourceView** pSRV);
	HRESULT Clear_RenderTarget(const wstring& strTargetTag);

	class CRenderTarget* Find_RenderTarget(const wstring& strTargetTag);

	//Release
	HRESULT Delete_RenderTarget(const wstring & strTargetTag);
	HRESULT Delete_MRT(const wstring & strMRTTag);

	/* strMRTTag에 해당하는 list에 담겨있는 타겟들을 장치에 바인딩한다. */
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, _bool bClear = true);
	HRESULT Begin_MRT(ID3D11DeviceContext* pContext, const wstring& strMRTTag, ID3D11DepthStencilView* pDSV);

	/* 다시 원래 상태로 복구한다. */
	HRESULT End_MRT(ID3D11DeviceContext* pContext);

public:
	HRESULT Ready_Debug(const wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(const wstring& strMRTTag, CShader* pShader, class CVIBuffer_Rect* pVIBuffer);


private:
	unordered_map<const wstring, CRenderTarget*, djb2Hasher>			m_RenderTargets;
	unordered_map<const wstring, vector<CRenderTarget*>, djb2Hasher>	m_MRTs;

private:
	ID3D11RenderTargetView*					m_pBackBufferRTV = { nullptr };
	ID3D11DepthStencilView*					m_pDSV = { nullptr };

private:
	vector<CRenderTarget*>* Find_MRT(const wstring & strMRTTag);

public:
	virtual void Free() override;
};

END