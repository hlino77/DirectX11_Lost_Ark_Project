#pragma once

/* ·»´õÅ¸°Ù ÇÏ³ª¸¦ ÀÇ¹ÌÇÏ³®. */
#include "Base.h"

BEGIN(Engine)

class CRenderTarget final : public CBase
{
private:
	CRenderTarget(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CRenderTarget() = default;
	
public:
	ID3D11RenderTargetView* Get_RTV() const {
		return m_pRTV;
	}

public:
	HRESULT Initialize(_uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const Vec4& vColor);
	HRESULT Clear();
	HRESULT	Bind_SRV(class CShader* pShader, const _char* pConstantName);
	HRESULT Make_TextureFile(const wstring& szPath);


#ifdef _DEBUG
public:
	HRESULT Ready_Debug(_float fX, _float fY, _float fSizeX, _float fSizeY);
	HRESULT Render(class CShader* pShader, class CVIBuffer_Rect* pBuffer);
	HRESULT	Copy_SRV(ID3D11ShaderResourceView** pSRV);

#endif

private:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

private:
	ID3D11Texture2D*			m_pTexture2D = { nullptr };	
	ID3D11RenderTargetView*		m_pRTV = { nullptr };
	ID3D11ShaderResourceView*	m_pSRV = { nullptr };

private:
	Vec4						m_vColor;


	_uint m_iSizeX;
	_uint m_iSizeY;
	DXGI_FORMAT m_ePixelFormat;


#ifdef _DEBUG
private:
	Matrix					m_WorldMatrix;
#endif

public:
	static CRenderTarget* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const Vec4& vColor);
	virtual void Free() override;
};

END