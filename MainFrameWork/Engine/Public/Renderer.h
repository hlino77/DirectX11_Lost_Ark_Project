#pragma once

#include "Component.h"
#include "Hasher.h"

/* 1. 화면에 그려져야하는 객체들을 그리는 순서대로 모아서 보관한다. */
/* 2. 보관하고 있는 객체들의 렌더콜(드로우콜)을 수행한다. */

BEGIN(Engine)

class CShader;
class CTexture;

class ENGINE_DLL CRenderer final : public CComponent
{
public:
	typedef struct MakeSRVDesc
	{
		CGameObject* pObject;
		ID3D11ShaderResourceView** pSRV;
	}MAKESRV;

public:
	enum RENDERGROUP { UPDATE_TEXTBOX, RENDER_STATICSHADOW, RENDER_PRIORITY, RENDER_NONLIGHT, RENDER_LIGHT, RENDER_NONBLEND, RENDER_SHADOW, RENDER_BLEND, RENDER_ALPHABLEND, RENDER_WORLDUI, RENDER_UI, RENDER_TEXTBOX, RENDER_END };

private:
	CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CRenderer(const CRenderer& rhs) = delete;
	virtual ~CRenderer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	HRESULT Reserve_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_RenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT	Add_InstanceRenderGroup(RENDERGROUP eRenderGroup, class CGameObject* pGameObject);
	HRESULT Add_MakeSRV(CGameObject* pObject, ID3D11ShaderResourceView** pSRV);
	HRESULT Add_DebugObject(CGameObject* pObject);

	HRESULT Ready_InstanceRender();
	HRESULT Draw();
	HRESULT Draw_Server();

	void	Set_StaticShadow() { m_bRenderStaticShadow = true; }

	HRESULT Bind_TextBoxSRV(CShader* pShader);
private:
	HRESULT Update_TextBox();
	HRESULT	Render_MakeSRV();

	HRESULT Render_Priority();

	HRESULT Render_StaticShadow();
	HRESULT Render_NonAlphaBlend();
	HRESULT	Render_ShadowDepth();
	HRESULT Render_Lights();
	HRESULT Render_LightAcc();

	HRESULT Render_SSAO();
	HRESULT Render_Deferred();

	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();

	HRESULT Render_Bloom();
	HRESULT Render_PostProcess();


	HRESULT Render_EffectBlur();
	HRESULT Render_EffectAcc();

	HRESULT	Render_WorldUI();
	HRESULT Render_UI();
	HRESULT	Render_TextBox();

	HRESULT Render_Debug();
	HRESULT Render_DebugObject();

private:

	//Debug
	vector<class CGameObject*> m_DebugRenderObjects;
	vector<class CGameObject*> m_RenderObjects[RENDER_END];
	unordered_map<wstring, vector<class CGameObject*>, djb2Hasher> m_InstanceRenderObjects[RENDER_END];

private:
	_uint	m_iBufferSize = 0;

	//RenderTarget
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CLight_Manager* m_pLight_Manager = { nullptr };

	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	CShader* m_pMRTShader = { nullptr };
	CShader* m_pPostProccessor = { nullptr };

	Matrix	m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

	//MakeSRV
	HRESULT	Ready_MakeSRV_DSV();
	ID3D11DepthStencilView* m_pMakeSRV_DSV = nullptr;
	vector<MAKESRV> m_MakeSRVObjects;

	//Shadow
	HRESULT	Ready_ShadowDSV();

	ID3D11DepthStencilView* m_pShadowDSV = nullptr;
	_float m_fShadowTargetSizeRatio;

	ID3D11DepthStencilView* m_pStaticShadowDSV = nullptr;
	_float m_fStaticShadowTargetSizeRatio;

	//Blur
	HRESULT	Ready_BlurData();

	Vec2	m_vPixelSize;
	_int	m_iKernelSize;
	vector<_float> m_BlurWeights;

	_float	m_fBias = 0.0000022f;

	_bool	m_bRenderStaticShadow = false;

	_bool	m_bTargetOnOff = false;
	_bool	m_bPBR_Switch = true;
	_int	m_iSSAO_Switch = true;

	//Bloom
	HRESULT	Ready_Bloom();
	HRESULT Execute_BloomBlur();


	//SSAO
	HRESULT Ready_SSAO();

	CShader* m_pSSAOShader = { nullptr };
	class CVIBuffer_RectSSAO* m_pVIBufferSSAO = { nullptr };

	Vec4 m_vFrustumFarCorner[4];
	Vec4 m_vOffsets[14];
	ID3D11ShaderResourceView* m_pRandomSRV = nullptr;

	struct SSAO_Data
	{
		Matrix	matViewToTexSpace; // Proj*Texture
		Vec4 vOffsetVectors[14];
		Vec4 vFrustumCorners[4];

		_float gOcclusionRadius = 0.5f;
		_float gOcclusionFadeStart = 0.2f;
		_float gOcclusionFadeEnd = 2.0f;
		_float gSurfaceEpsilon = 0.05f;
	}; 

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	const XMMATRIX m_matToTexture =
	{	0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f	};
	
	class CTexture* m_pRandomTexture = nullptr;

	// Bloom
	ID3D11DepthStencilView* m_pDownSample4x4_DSV = nullptr;
	ID3D11DepthStencilView* m_pDownSample24x24_DSV = nullptr;
	ID3D11DepthStencilView* m_pDownSample144x144_DSV = nullptr;
	CShader* m_pBloomShader = { nullptr };
	struct tagPerFrame
	{
		_float	fTexelWidth = 1.f / 1600.f;
		_float	fTexelHeight = 1.f / 900.f;
		Vec2	vPadding;
	};

	_float m_fSampleRatio4x4 = 4.f;
	_float m_fSampleRatio16x16 = 24.f;
	_float m_fSampleRatio64x64 = 144.f;

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pObject, void* pArg) override;
	virtual void Free() override;

};

END