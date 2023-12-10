#pragma once

#include "Base.h"
#include "AsTypes.h"

BEGIN(Engine)

class CGraphic_Device final : public CBase
{
	DECLARE_SINGLETON(CGraphic_Device)

private:
	CGraphic_Device();
	virtual ~CGraphic_Device() = default;

public:
	/* 그래픽 디바이스의 초기화. */
	HRESULT Ready_Graphic_Device(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode,
		_uint iWinCX, _uint iWinCY, _Inout_ ID3D11Device** ppDevice,
		_Inout_ ID3D11DeviceContext** ppDeviceContextOut);

	/* 백버퍼를 지운다. */
	HRESULT Clear_BackBuffer_View(Vec4 vClearColor);

	/* 깊이버퍼 + 스텐실버퍼를 지운다. */
	HRESULT Clear_DepthStencil_View();

	/* 후면 버퍼를 전면버퍼로 교체한다.(백버퍼를 화면에 직접 보여준다.) */
	HRESULT Present();

	ID3D11Device* Get_Device() { return m_pDevice; }
	ID3D11DeviceContext* Get_Context() { return m_pDeviceContext; }

private:		
	// IDirect3DDevice9* == LPDIRECT3DDEVICE9 == ID3D11Device + ID3D11DeviceContext 

	/* 메모리 할당. (정점버퍼, 인덱스버퍼, 텍스쳐로드) 컴객체의 생성과 관련된 역활.  */
	ID3D11Device*				m_pDevice = nullptr;
		
	/* 기능실행.(바인딩작업, 정점버퍼를 SetStreamSource(), SetIndices(), SetTransform(), SetTexture() */
	/* 그린다. */
	ID3D11DeviceContext*		m_pDeviceContext = nullptr;	

	/* 후면버퍼와 전면버퍼를 교체해가면서 화면에 보여주는 역활. */
	IDXGISwapChain*				m_pSwapChain = nullptr;		


	/* IDirect3DTexture9 */
	/* ID3D11Texture2D : 텍스쳐를 표현하는 사전객체 타입이다.
	why? 용도에 맞는 실질적으로 사용하기위한 텍스쳐객체를 만들어내기위해.  */	


	/* ID3D11ShaderResourceView : 셰이더에 전달될 수 있는 테긋쳐타입. */	
	/* ID3D11RenderTargetView : 렌더타겟용으로 사용될 수 있는 텍스쳐 타입. */
	/* ID3D11DepthStencilView : 깊이스텐실버퍼로서 사용될 수 있는 타입.  */
	ID3D11RenderTargetView*		m_pBackBufferRTV = nullptr;
	ID3D11DepthStencilView*		m_pDepthStencilView = nullptr;

	
	
private:
	/* 스왑체인에게 필수적으로 필요한 데이터는 백버퍼가 필요하여. 하여 백버퍼를 생성하기위한 정보를 던져준다. */
	/* 스왑체인을 만들었다 == 백버퍼(텍스쳐)가 생성된다. */
	HRESULT Ready_SwapChain(HWND hWnd, GRAPHIC_DESC::WINMODE eWinMode, _uint iWinCX, _uint iWinCY);
	HRESULT Ready_BackBufferRenderTargetView();
	HRESULT Ready_DepthStencilRenderTargetView(_uint iWinCX, _uint iWinCY);

public:
	virtual void Free() override;
};

END