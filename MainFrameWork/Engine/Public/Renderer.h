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
	enum RENDERGROUP { UPDATE_TEXTBOX, RENDER_STATICSHADOW, RENDER_PRIORITY, RENDER_NONLIGHT, RENDER_LIGHT, RENDER_NONBLEND, RENDER_DECAL, RENDER_EFFECT, RENDER_EFFECTPARTICLE, RENDER_SHADOW, RENDER_BLEND, RENDER_ALPHABLEND, RENDER_WORLDUI, RENDER_UI, RENDER_TEXTBOX, RENDER_MOUSE, RENDER_OUTLINE, RENDER_ESHTER, RENDER_END };

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
	static void	Set_IBLTexture(_int iIndex) { m_iIBLTextureIndex = iIndex; }

	static void	Set_SSRLevel(_int iLevel) { m_iSSRLevel = iLevel; }
	static void	Set_GrayScale(_float fGrayScale) { m_tScreenTone_Data.fGrayScale = fGrayScale; }
	static void	Set_Contrast(_float fContrast) { m_tScreenTone_Data.fContrast = fContrast; }
	static void	Set_Saturation(_float fSaturation) { m_tScreenTone_Data.fSaturation = fSaturation; }

	_int	Get_IBLTexture() { return m_iIBLTextureIndex; }
	_int	Get_SSRLevel() { return m_iSSRLevel; }
	_float	Get_GrayScale() { return m_tScreenTone_Data.fGrayScale; }
	_float	Get_Contrast() { return m_tScreenTone_Data.fContrast; }
	_float	Get_Saturation() { return m_tScreenTone_Data.fSaturation; }

	void	Set_RadialBlurData(Vec3 vWorldPos, _float fIntensity);
	void	Set_ChromaticData(Vec3 vWorldPos, _float fIntensity);

	HRESULT Bind_TextBoxSRV(CShader* pShader);

	void	Set_DeadScene(_bool bSetDead) { m_bDeadScene = bSetDead; }

	void	Set_ScreenShot(_bool bShoot, wstring strPath = TEXT(""));

	static void	Set_SSAO_Switch(_bool bSwitch) { m_iSSAO_Switch = bSwitch; }
	static void	Set_PBR_Switch (_bool bSwitch) { m_bPBR_Switch = bSwitch; }
	static void	Set_Fxaa_Switch(_bool bSwitch) { m_iFxaa_Switch = bSwitch; }
	
	_bool	Get_SSAO_Switch() {return m_iSSAO_Switch; }
	_bool	Get_PBR_Switch() { return m_bPBR_Switch; }
	_bool	Get_Fxaa_Switch() {return m_iFxaa_Switch; }
	// For Fog
	static void Set_Fog_StartHeight(_float Height) { m_fFogStartHeight = Height; }
	static void Set_Fog_EndHeight(_float Height) { m_fFogEndHeight = Height; }
	static void Set_Fog_Density(_float Density) { m_fFogDensity = Density; }
	static void Set_Fog_Color(Vec3 Color) { m_vFogColor = Color; }
	static void Set_Fog_Time(_float fTime) { m_fFogTime = fTime; }
	static void Set_Add_Fog_Time(_float fTime) { m_fFogTime += fTime; }
	static void Set_Fog_ChangeSpeed(_float fSpeed) { m_fFogChangeSpeed = fSpeed; }
	static void Set_Fog_MinValue(_float fMinValue) { m_fFogMinValue = fMinValue; }

private:
	HRESULT Update_TextBox();
	HRESULT	Render_MakeSRV();

	HRESULT Render_Priority();

	HRESULT Render_StaticShadow();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_Decal();
	HRESULT Render_Effect();
	HRESULT	Render_ShadowDepth();
	HRESULT Render_Lights();

	HRESULT Render_SSR();

	HRESULT Render_SSAO();
	HRESULT Render_Deferred();

	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();

	HRESULT Render_Bloom();
	HRESULT Render_PostProcess();
	HRESULT Render_FXAA();

	HRESULT	Render_WorldUI();
	HRESULT Render_UI();
	HRESULT	Render_Mouse();

	HRESULT Render_Debug();
	HRESULT Render_DebugObject();

	HRESULT Render_Outline();

	HRESULT Render_Esther();

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
	
	// Deferred
	CShader* m_pMRTShader = { nullptr };

	// Deferred for Fog
	static  Vec3    m_vFogColor;
	static  _float 	m_fFogStartHeight;
	static  _float  m_fFogEndHeight;
	static  _float	m_fFogDensity;
	static  _float  m_fFogTime;
	static  _float  m_fFogChangeSpeed;
	static  _float  m_fFogMinValue;

	// PostProccess
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
	static _bool	m_bPBR_Switch;
	static _int		m_iSSAO_Switch;

	//Bloom
	HRESULT	Ready_Bloom();
	HRESULT Execute_BloomBlur();

	//SSAO
	HRESULT Ready_SSAO();

	CShader* m_pSSAOShader = nullptr;
	class CVIBuffer_RectSSAO* m_pVIBufferSSAO = nullptr;

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
	} m_tSSAO_Data;

	// Transform NDC space [-1,+1]^2 to texture space [0,1]^2
	const XMMATRIX m_matToTexture =
	{	0.5f, 0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f	};
	
	class CTexture* m_pRandomTexture = nullptr;

	// Bloom
	ID3D11DepthStencilView* m_pDownSample5x5_DSV = nullptr;
	ID3D11DepthStencilView* m_pDownSample25x25_DSV = nullptr;
	ID3D11DepthStencilView* m_pDownSample125x125_DSV = nullptr;
	CShader* m_pBloomShader =  nullptr;
	struct tagPerFrame
	{
		_float	fTexelWidth = 1.f / 1600.f;
		_float	fTexelHeight = 1.f / 900.f;
		Vec2	vPadding;
	};

	_float m_fSampleRatio5x5 = 5.f;
	_float m_fSampleRatio25x25 = 25.f;
	_float m_fSampleRatio125x125 = 125.f;

	// IBL
	HRESULT		Ready_IBL();
	CTexture*	m_pIrradianceTexture = nullptr;
	CTexture*	m_pPreFilteredTexture = nullptr;
	CTexture*	m_pBRDFTexture = nullptr;

	static _uint	m_iIBLTextureIndex;

	// Screen Space Reflection
	HRESULT		Ready_SSR();
	CShader* m_pSSRShader = nullptr;
	struct SSR_Data
	{
		_float	fSSRStep = 0.01f;
		_int	iSSRStepCount = 50;
		Vec2	vPadding;
	} m_tSSR_Data[5];

	static _uint	m_iSSRLevel;

	// Chromatic Aberration
	struct ChromaticBlur_Data
	{
		Vec3	vChromaticWorldPos = Vec3(0.f, 0.f, 0.f);
		_float	fChromaticIntensity = 0.f;
	} m_tChromatic_Data;

	// Motion Blur
	Matrix	m_matPreCamView = Matrix::Identity;
	_bool	m_bMotionBlurInitialized = false;

	// Radial Blur
	struct RadialBlur_Data
	{
		Vec3	vRadialBlurWorldPos = Vec3(0.f, 0.f, 0.f);
		_float	fRadialBlurIntensity = 0.f;
	} m_tRadialBlur_Data;

	// Fxaa3_11
	CShader* m_pFxaaShader = nullptr;
	static _int	m_iFxaa_Switch;

	// Eshter Motion

	// Screen Tone
	struct ScreenTone_Data
	{
		_float	fGrayScale = 1.f;
		_float	fContrast = 1.f;
		_float	fSaturation = 1.f;
		_float	fPadding;
	};

	static ScreenTone_Data m_tScreenTone_Data;
	ScreenTone_Data m_tScreenTone_BackUp;

	_bool	m_bDeadScene = { false };
	_bool	m_bDeadSceneStart = { false };

	// ScreenShot
	_bool	m_bScreenShot = { false };
	wstring m_strScreenShotPath;
	_uint	m_iScreenShotFrame = { 0 };

	static  _float  m_fGrayScale;
	static  _float  m_fContrast;
	static  _float  m_fSaturation;

public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pObject, void* pArg) override;
	virtual void Free() override;

};

END