#pragma once

#include "Component.h"

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
	enum RENDERGROUP { RENDER_STATICSHADOW, RENDER_PRIORITY, RENDER_NONLIGHT, RENDER_LIGHT, INSTANCE_STATIC, RENDER_NONBLEND, RENDER_SHADOW, RENDER_BLEND, RENDER_MODELEFFECT_INSTANCE, RENDER_EFFECT_INSTANCE, RENDER_ALPHABLEND, RENDER_WORLDUI, RENDER_UI, RENDER_END };

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
	HRESULT Add_MakeSRV(CGameObject* pObject, ID3D11ShaderResourceView** pSRV);


	HRESULT Draw();
	HRESULT Draw_Server();


	void	Set_StaticShadow() { m_bRenderStaticShadow = true; }

private:

	HRESULT	Render_MakeSRV();


	HRESULT Render_Priority();

	HRESULT Render_StaticShadow();
	HRESULT Render_NonAlphaBlend();
	HRESULT Render_StaticInstance();
	HRESULT	Render_ShadowDepth();
	HRESULT Render_Lights();
	HRESULT Render_LightAcc();
	HRESULT Render_Deferred();


	HRESULT Render_Blend();
	HRESULT Render_NonLight();
	HRESULT Render_AlphaBlend();
	HRESULT Render_ModelEffectInstance();
	HRESULT	Render_EffectInstance();

	HRESULT Render_EffectBlur();
	HRESULT Render_EffectAcc();

	HRESULT	Render_WorldUI();
	HRESULT Render_UI();


	HRESULT Render_Debug();
private:
	HRESULT Render_ModelInstancing(const wstring& szModelName);
	HRESULT Render_EffectInstancing(const wstring& szModelName);
	HRESULT Render_ModelEffectInstancing(const wstring& szModelName);



	HRESULT Ready_InstanceBuffer();


	list<class CGameObject*>			m_RenderObjects[RENDER_END];
	unordered_map<wstring, list<class CGameObject*>> m_StaticInstance;
	unordered_map<wstring, list<class CGameObject*>> m_EffectInstance;
	unordered_map<wstring, list<class CGameObject*>> m_ModelEffectInstance;
private:
	
	CShader* m_pInstanceShader = nullptr;

	ID3D11Buffer* m_pInstanceBuffer = nullptr;
	ID3D11Buffer* m_pPointEffect_InstanceBuffer = nullptr;
	ID3D11Buffer* m_pModelEffect_InstanceBuffer = nullptr;




	_uint	m_iBufferSize = 0;


	//RenderTarget
	class CTarget_Manager* m_pTarget_Manager = { nullptr };
	class CLight_Manager* m_pLight_Manager = { nullptr };

	class CVIBuffer_Rect* m_pVIBuffer = { nullptr };
	class CShader* m_pMRTShader = { nullptr };
	class CShader* m_pEffectShader = { nullptr };

	Matrix					m_WorldMatrix, m_ViewMatrix, m_ProjMatrix;

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
	void	Ready_BlurData();

	Vec2	m_vPixelSize;
	_int	m_iKernelSize;
	vector<_float> m_BlurWeights;

	_float m_fBias = 0.0000022f;


	_bool m_bRenderStaticShadow = false;
public:
	static CRenderer* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(CGameObject* pObject, void* pArg) override;
	virtual void Free() override;

};

END