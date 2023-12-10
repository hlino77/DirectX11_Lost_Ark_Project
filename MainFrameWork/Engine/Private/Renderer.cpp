#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "GameInstance.h"
#include "NavigationMgr.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "Texture.h"

CRenderer::CRenderer(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CComponent(pDevice, pContext)
	, m_pTarget_Manager(CTarget_Manager::GetInstance())
	, m_pLight_Manager(CLight_Manager::GetInstance())
{
	Safe_AddRef(m_pTarget_Manager);
	Safe_AddRef(m_pLight_Manager);
}

HRESULT CRenderer::Initialize_Prototype()
{
	if (m_pDevice == nullptr)
		return S_OK;

	Ready_InstanceBuffer();

	//RenderTarget

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_fShadowTargetSizeRatio = 5.12f;
	m_fStaticShadowTargetSizeRatio = 5.12f;


	/* For.Target_Diffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	/* For.Target_MakeSRV */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_MakeSRV"),
		500, 500, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;


	/* For.Target_Normal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Shade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_Depth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Depth"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_ModelNormal */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ModelNormal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"),
		ViewportDesc.Width * m_fShadowTargetSizeRatio, ViewportDesc.Height * m_fShadowTargetSizeRatio, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	/* For.Target_ShadowDepth */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_StaticShadowDepth"),
		ViewportDesc.Width * m_fStaticShadowTargetSizeRatio, ViewportDesc.Height * m_fStaticShadowTargetSizeRatio, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;


	/* For.Target_EffectDiffuse */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectDiffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;


	/* For.Target_Blur */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectBlur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectBlurX"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;


	/* For.Target_EffectShade */
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectShade"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;



#ifdef _DEBUG
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Diffuse"), 75.f, 75.f, 150.0f, 150.0f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), 75.f, 225.0f, 150.0f, 150.0f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 225.0f, 75.f, 150.0f, 150.0f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Depth"), 225.0f, 225.0f, 150.0f, 150.0f)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ShadowDepth"), 180.0f, 180.0f, 360.0f, 360.0f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_StaticShadowDepth"), 180.0f, 540.0f, 360.0f, 360.0f)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_ModelNormal"), 150.0f, 570.0f, 300.0f, 300.0f)))
		return E_FAIL;

	
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectDiffuse"), 75.f, 525.f, 150.0f, 150.0f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectBlur"), 225.f, 525.f, 150.0f, 150.0f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectBlurX"), 525.f, 525.f, 150.0f, 150.0f)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectShade"), 375.f, 525.f, 150.0f, 150.0f)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_MakeSRV"), 1450.f, 150.0f, 150.0f, 150.0f)))
		return E_FAIL;

	

#endif

	/* 이 렌더타겟들은 그려지는 객체로부터 값을 저장받는다. */
	/* For.MRT_GameObjects */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Depth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_ModelNormal"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_StaticShadowDepth"), TEXT("Target_StaticShadowDepth"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_MakeSRV"), TEXT("Target_MakeSRV"))))
		return E_FAIL;



	/* 이 렌더타겟들은 게임내에 존재하는 빛으로부터 연산한 결과를 저장받는다. */
	/* For.MRT_ */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;




	//BlurEffect
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_EffectDiffuse"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_EffectBlur"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_EffectBlurX"), TEXT("Target_EffectBlurX"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_EffectBlurY"), TEXT("Target_EffectBlur"))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_EffectShade"), TEXT("Target_EffectShade"))))
		return E_FAIL;



	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;

	m_pMRTShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pMRTShader)
		return E_FAIL;

	m_pEffectShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred_Effect.hlsl"), VTXTEX_DECLARATION::Elements, VTXTEX_DECLARATION::iNumElements);
	if (nullptr == m_pEffectShader)
		return E_FAIL;

	Ready_MakeSRV_DSV();
	Ready_ShadowDSV();


	m_WorldMatrix = XMMatrixIdentity();
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	m_ViewMatrix = XMMatrixIdentity();
	
	m_ProjMatrix = XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f);



	Ready_BlurData();

	return S_OK;
}

HRESULT CRenderer::Initialize(void * pArg)
{
	return S_OK;
}

HRESULT CRenderer::Reserve_RenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	if (eRenderGroup == RENDERGROUP::INSTANCE_STATIC)
	{
		m_StaticInstance[pGameObject->Get_ModelName()].clear();
		Safe_AddRef(pGameObject);
		return S_OK;
	}
	else if (eRenderGroup == RENDERGROUP::RENDER_EFFECT_INSTANCE)
	{
		m_EffectInstance[pGameObject->Get_ModelName()].clear();
		Safe_AddRef(pGameObject);
		return S_OK;
	}
	else if (eRenderGroup == RENDERGROUP::RENDER_MODELEFFECT_INSTANCE)
	{
		m_ModelEffectInstance[pGameObject->Get_ModelName()].clear();
		Safe_AddRef(pGameObject);
		return S_OK;
	}

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	if (eRenderGroup == RENDERGROUP::INSTANCE_STATIC)
	{
		m_StaticInstance[pGameObject->Get_ModelName()].push_back(pGameObject);
		Safe_AddRef(pGameObject);
		return S_OK;
	}
	else if (eRenderGroup == RENDERGROUP::RENDER_EFFECT_INSTANCE)
	{
		m_EffectInstance[pGameObject->Get_ModelName()].push_back(pGameObject);
		Safe_AddRef(pGameObject);
		return S_OK;
	}
	else if (eRenderGroup == RENDERGROUP::RENDER_MODELEFFECT_INSTANCE)
	{
		m_ModelEffectInstance[pGameObject->Get_ModelName()].push_back(pGameObject);
		Safe_AddRef(pGameObject);
		return S_OK;
	}



	m_RenderObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_MakeSRV(CGameObject* pObject, ID3D11ShaderResourceView** pSRV)
{
	MAKESRV tDesc;
	tDesc.pObject = pObject;
	tDesc.pSRV = pSRV;
	m_MakeSRVObjects.push_back(tDesc);

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	if (m_bRenderStaticShadow)
		Render_StaticShadow();

	Render_MakeSRV();


	Render_Priority();

	Render_NonAlphaBlend();
	Render_StaticInstance();
	Render_ShadowDepth();


	Render_Lights();
	Render_LightAcc();
	Render_Deferred();


	Render_Blend();
	Render_NonLight();
	Render_AlphaBlend();
	Render_ModelEffectInstance();
	Render_EffectInstance();

	Render_EffectBlur();
	Render_EffectAcc();


	Render_WorldUI();
	Render_UI();

	//Render_Debug();
	
	
	return S_OK;
}

HRESULT CRenderer::Draw_Server()
{
	for (auto& iter : m_RenderObjects[RENDER_NONBLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_MakeSRV()
{
	if (m_MakeSRVObjects.empty())
		return S_OK;


	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float fOriginalWidth = ViewportDesc.Width;
	_float fOriginalHeight = ViewportDesc.Height;

	ViewportDesc.Width = 500;
	ViewportDesc.Height = 500;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);



	for (auto& iter : m_MakeSRVObjects)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_MakeSRV"), m_pMakeSRV_DSV)))
			return E_FAIL;

		m_pContext->ClearDepthStencilView(m_pMakeSRV_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		if (FAILED(iter.pObject->Render_MakeSRV()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;



		m_pTarget_Manager->Copy_SRV(L"Target_MakeSRV", iter.pSRV);

	}
	m_MakeSRVObjects.clear();


	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	ViewportDesc.Width = fOriginalWidth;
	ViewportDesc.Height = fOriginalHeight;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	for (auto& iter : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

	return S_OK;
}

HRESULT CRenderer::Render_StaticInstance()
{
	for (auto& iter : m_StaticInstance)
	{
		Render_ModelInstancing(iter.first);
		iter.second.clear();
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ShadowDepth()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_ShadowDepth"), m_pShadowDSV)))
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float fOriginalWidth = ViewportDesc.Width;
	_float fOriginalHeight = ViewportDesc.Height;

	ViewportDesc.Width *= m_fShadowTargetSizeRatio;
	ViewportDesc.Height *= m_fShadowTargetSizeRatio;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);


	for (auto& iter : m_RenderObjects[RENDER_SHADOW])
	{
		if (FAILED(iter->Render_ShadowDepth()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_SHADOW].clear();


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	ViewportDesc.Width = fOriginalWidth;
	ViewportDesc.Height = fOriginalHeight;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Render_StaticShadow()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_StaticShadowDepth"), m_pStaticShadowDSV)))
		return E_FAIL;

	m_pContext->ClearDepthStencilView(m_pStaticShadowDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	D3D11_VIEWPORT		ViewportDesc;
	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float fOriginalWidth = ViewportDesc.Width;
	_float fOriginalHeight = ViewportDesc.Height;

	ViewportDesc.Width *= m_fStaticShadowTargetSizeRatio;
	ViewportDesc.Height *= m_fStaticShadowTargetSizeRatio;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);


	for (auto& iter : m_RenderObjects[RENDER_STATICSHADOW])
	{
		if (FAILED(iter->Render_ShadowDepth()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_STATICSHADOW].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	m_pTarget_Manager->Make_SRVTexture(L"../Bin/Resources/Textures/LightMap/LightMap.dds", L"Target_StaticShadowDepth");


	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	ViewportDesc.Width = fOriginalWidth;
	ViewportDesc.Height = fOriginalHeight;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	m_bRenderStaticShadow = false;

	return S_OK;
}

HRESULT CRenderer::Render_NonAlphaBlend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_GameObjects"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_NONBLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
	}
	
	m_RenderObjects[RENDER_NONBLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Lights()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_LIGHT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_LIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_LightAcc()
{

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Lights"))))
		return E_FAIL;


	/* 사각형 버퍼를 직교투영으로 Shade타겟의 사이즈만큼 꽉 채워서 그릴꺼야. */
	if (FAILED(m_pMRTShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;



	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrixInv", &CPipeLine::GetInstance()->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrixInv", &CPipeLine::GetInstance()->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pMRTShader->Bind_Matrix("g_LightViewMatrix", &CLight_Manager::GetInstance()->Get_DirectionLightMatrix())))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_LightProjMatrix", &CPipeLine::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pMRTShader->Bind_Matrix("g_StaticLightViewMatrix", &CLight_Manager::GetInstance()->Get_StaticLightMatrix())))
		return E_FAIL;


	D3D11_VIEWPORT		ViewportDesc;
	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);
	Vec2 vWinSize(ViewportDesc.Width, ViewportDesc.Height);


	/*if (KEY_TAP(KEY::UP_ARROW))
		m_fBias += 0.000001f;
	if (KEY_TAP(KEY::DOWN_ARROW))
		m_fBias -= 0.000001f;*/


	if (FAILED(m_pMRTShader->Bind_RawValue("g_fBias", &m_fBias, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_vWinSize", &vWinSize, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fShadowSizeRatio", &m_fShadowTargetSizeRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fStaticShadowSizeRatio", &m_fStaticShadowTargetSizeRatio, sizeof(_float))))
		return E_FAIL;


	Vec3 vCamPos = CPipeLine::GetInstance()->Get_CamPosition();

	if (FAILED(m_pMRTShader->Bind_RawValue("g_CamPosition", &vCamPos, sizeof(Vec3))))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Normal"), "g_NormalTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Depth"), "g_DepthTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_ModelNormal"), "g_ModelNormalTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_ShadowDepth"), "g_ShadowDepthTexture")))
		return E_FAIL;


	

	m_pLight_Manager->Render(m_pMRTShader, m_pVIBuffer);


	/* 다시 장치의 0번째 소켓에 백 버퍼를 올린다. */
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
	/* 디퓨즈 타겟과 셰이드 타겟을 서로 곱하여 백버퍼에 최종적으로 찍어낸다. */
	if (FAILED(m_pMRTShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Diffuse"), "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Shade"), "g_ShadeTexture")))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	
	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effects"))))
		return E_FAIL;


	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_BLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_BLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_NonLight()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_NONLIGHT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_NONLIGHT].clear();

	return S_OK;
}

HRESULT CRenderer::Render_AlphaBlend()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_ALPHABLEND])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_ALPHABLEND].clear();

	return S_OK;
}

HRESULT CRenderer::Render_ModelEffectInstance()
{
	for (auto& iter : m_ModelEffectInstance)
	{
		if (!iter.second.empty())
		{
			Render_ModelEffectInstancing(iter.first);
			iter.second.clear();
		}
	}

	return S_OK;
}

HRESULT CRenderer::Render_EffectInstance()
{
	for (auto& iter : m_EffectInstance)
	{
		if (!iter.second.empty())
		{
			Render_EffectInstancing(iter.first);
			iter.second.clear();
		}
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_EffectBlur()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_EffectBlurX"))))
		return E_FAIL;


	if (FAILED(m_pEffectShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffectShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffectShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pEffectShader, TEXT("Target_EffectBlur"), "g_BlurTexture")))
		return E_FAIL;

	if (FAILED(m_pEffectShader->Bind_RawValue("g_PixelSize", &m_vPixelSize, sizeof(Vec2))))
		return E_FAIL;

	if (FAILED(m_pEffectShader->Bind_RawValue("g_KernelSize", &m_iKernelSize, sizeof(_int))))
		return E_FAIL;

	_float fCenterWeight = 0.25f;
	_float fWeightAtt = 0.025f;

	if (FAILED(m_pEffectShader->Bind_RawValue("g_CenterWeight", &fCenterWeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pEffectShader->Bind_RawValue("g_WeightAtt", &fWeightAtt, sizeof(_float))))
		return E_FAIL;


	if (FAILED(m_pEffectShader->Begin(2)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	/* 다시 장치의 0번째 소켓에 백 버퍼를 올린다. */
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_EffectBlurY"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pEffectShader, TEXT("Target_EffectBlurX"), "g_BlurTexture")))
		return E_FAIL;
	

	if (FAILED(m_pEffectShader->Begin(3)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_EffectAcc()
{
	/*if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_EffectShade"))))
		return E_FAIL;*/


	/* 사각형 버퍼를 직교투영으로 Shade타겟의 사이즈만큼 꽉 채워서 그릴꺼야. */
	if (FAILED(m_pEffectShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffectShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pEffectShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	


	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pEffectShader, TEXT("Target_EffectBlur"), "g_DiffuseTexture")))
		return E_FAIL;


	if (FAILED(m_pEffectShader->Begin(4)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;



	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pEffectShader, TEXT("Target_EffectDiffuse"), "g_DiffuseTexture")))
		return E_FAIL;

	if (FAILED(m_pEffectShader->Begin(1)))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;




	/* 다시 장치의 0번째 소켓에 백 버퍼를 올린다. */
	/*if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;*/


	return S_OK;
}


HRESULT CRenderer::Render_WorldUI()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_WORLDUI])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_WORLDUI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_UI()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_UI])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_UI].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{

	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GameObjects"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;


	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effects"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_EffectShade"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_ShadowDepth"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_StaticShadowDepth"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_StaticShadowDepth"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_ModelInstancing(const wstring& szModelName)
{
	if (nullptr == m_pInstanceShader)
		return S_OK;

	if (m_StaticInstance[szModelName].empty())
		return S_OK;

	vector<Matrix> WorldMatrix;
	WorldMatrix.reserve(500);

	for (auto& Model : m_StaticInstance[szModelName])
	{
		WorldMatrix.push_back(Model->Get_TransformCom()->Get_WorldMatrix());
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	m_pContext->Map(m_pInstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, WorldMatrix.data(), sizeof(Matrix) * WorldMatrix.size());

	m_pContext->Unmap(m_pInstanceBuffer, 0);


	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (FAILED(m_pInstanceShader->Bind_Matrix("g_ViewMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return S_OK;
	if (FAILED(m_pInstanceShader->Bind_Matrix("g_ProjMatrix", &pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return S_OK;

	CModel* pModel = m_StaticInstance[szModelName].front()->Get_ModelCom();

	_uint		iNumMeshes = pModel->Get_NumMeshes();

	for (_uint i = 0; i < iNumMeshes; ++i)
	{
		if (FAILED(pModel->SetUp_OnShader(m_pInstanceShader, pModel->Get_MaterialIndex(i), aiTextureType_DIFFUSE, "g_DiffuseTexture")))
			return S_OK;

		/*if (FAILED(m_pModelCom->SetUp_OnShader(m_pShaderCom, m_pModelCom->Get_MaterialIndex(i), aiTextureType_NORMALS, "g_NormalTexture")))
			return E_FAIL;*/


		if (FAILED(pModel->Render_Instance(m_pInstanceBuffer, WorldMatrix.size(), m_pInstanceShader, i)))
			return S_OK;
	}

	Safe_Release(pGameInstance);


	return S_OK;
}

HRESULT CRenderer::Render_EffectInstancing(const wstring& szModelName)
{

	/*Vec3 vCamPos = CGameInstance::GetInstance()->Get_CamPosition();


	::sort(m_EffectInstance[szModelName].begin(), m_EffectInstance[szModelName].end(), [&](CGameObject* pObjL, CGameObject* pObjR) ->_bool
		{

			_float fDistanceL = (vCamPos - pObjL->Get_TransformCom()->Get_State(CTransform::STATE_POSITION)).Length();
			_float fDistanceR = (vCamPos - pObjR->Get_TransformCom()->Get_State(CTransform::STATE_POSITION)).Length();

			return fDistanceL < fDistanceR;
		});*/



	vector<Vec4> InstanceData;
	InstanceData.reserve(5000);

	_uint iSize = 0;
	for (auto& Object : m_EffectInstance[szModelName])
	{
		Object->Add_InstanceData(InstanceData);
		++iSize;
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	m_pContext->Map(m_pPointEffect_InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, InstanceData.data(), sizeof(Vec4) * InstanceData.size());

	m_pContext->Unmap(m_pPointEffect_InstanceBuffer, 0);

	m_EffectInstance[szModelName].front()->Render_Instance(m_pPointEffect_InstanceBuffer, iSize);

	return S_OK;
}

HRESULT CRenderer::Render_ModelEffectInstancing(const wstring& szModelName)
{
	vector<Vec4> InstanceData;
	InstanceData.reserve(3000);

	_uint iSize = 0;
	for (auto& Object : m_ModelEffectInstance[szModelName])
	{
		Object->Add_InstanceData(InstanceData);
		++iSize;
	}

	D3D11_MAPPED_SUBRESOURCE		SubResource = {};

	m_pContext->Map(m_pModelEffect_InstanceBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &SubResource);

	memcpy(SubResource.pData, InstanceData.data(), sizeof(Vec4) * InstanceData.size());

	m_pContext->Unmap(m_pModelEffect_InstanceBuffer, 0);

	m_ModelEffectInstance[szModelName].front()->Render_Instance(m_pModelEffect_InstanceBuffer, iSize);

	return S_OK;
}

HRESULT CRenderer::Ready_InstanceBuffer()
{
	//Instance
	m_pInstanceShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Vtx_Instance.hlsl"), VTX_MODEL_INSTANCE::Elements, VTX_MODEL_INSTANCE::iNumElements);

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));


		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(VTXINSTANCE) * 500;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(VTXINSTANCE);

		D3D11_SUBRESOURCE_DATA		InitialData;

		vector<Matrix> InitMatrix;

		InitMatrix.resize(500, XMMatrixIdentity());

		InitialData.pSysMem = InitMatrix.data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pInstanceBuffer)))
			return E_FAIL;
	}

	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));


		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(VTXINSTANCE_POINTEFFECT) * 1000;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(VTXINSTANCE_POINTEFFECT);

		D3D11_SUBRESOURCE_DATA		InitialData;

		vector<Vec4> InitData;

		InitData.resize(5000, Vec4(0.0f, 0.0f, 0.0f, 0.0f));

		InitialData.pSysMem = InitData.data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pPointEffect_InstanceBuffer)))
			return E_FAIL;


	}



	{
		D3D11_BUFFER_DESC			BufferDesc;

		ZeroMemory(&BufferDesc, sizeof(D3D11_BUFFER_DESC));


		// m_BufferDesc.ByteWidth = 정점하나의 크기(Byte) * 정점의 갯수;
		BufferDesc.ByteWidth = sizeof(VTXINSTANCE_MODELEFFECT) * 500;
		BufferDesc.Usage = D3D11_USAGE_DYNAMIC; /* 정적버퍼로 할당한다. (Lock, unLock 호출 불가)*/
		BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		BufferDesc.MiscFlags = 0;
		BufferDesc.StructureByteStride = sizeof(VTXINSTANCE_MODELEFFECT);

		D3D11_SUBRESOURCE_DATA		InitialData;

		vector<Vec4> InitData;

		InitData.resize(3000, Vec4(0.0f, 0.0f, 0.0f, 0.0f));

		InitialData.pSysMem = InitData.data();

		if (FAILED(m_pDevice->CreateBuffer(&BufferDesc, &InitialData, &m_pModelEffect_InstanceBuffer)))
			return E_FAIL;
	}

	return S_OK;
}


HRESULT CRenderer::Ready_MakeSRV_DSV()
{
	{
		ID3D11Texture2D* pDepthStencilTexture = nullptr;

		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = 500;
		TextureDesc.Height = 500;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pMakeSRV_DSV)))
			return E_FAIL;
	}

	return E_NOTIMPL;
}

HRESULT CRenderer::Ready_ShadowDSV()
{
	
	
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	
	//ShadowDepth
	{
		ID3D11Texture2D* pDepthStencilTexture = nullptr;

		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = m_fShadowTargetSizeRatio * ViewportDesc.Width;
		TextureDesc.Height = m_fShadowTargetSizeRatio * ViewportDesc.Height;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pShadowDSV)))
			return E_FAIL;
	}

	//StaticShadowDepth
	{
		ID3D11Texture2D* pDepthStencilTexture = nullptr;

		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = m_fStaticShadowTargetSizeRatio * ViewportDesc.Width;
		TextureDesc.Height = m_fStaticShadowTargetSizeRatio * ViewportDesc.Height;
		TextureDesc.MipLevels = 1;
		TextureDesc.ArraySize = 1;
		TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

		TextureDesc.SampleDesc.Quality = 0;
		TextureDesc.SampleDesc.Count = 1;

		TextureDesc.Usage = D3D11_USAGE_DEFAULT;
		TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
		TextureDesc.CPUAccessFlags = 0;
		TextureDesc.MiscFlags = 0;

		if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
			return E_FAIL;

		if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pStaticShadowDSV)))
			return E_FAIL;
	}


	return S_OK;
}

void CRenderer::Ready_BlurData()
{
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_vPixelSize.x = 1.0f / ViewportDesc.Width;
	m_vPixelSize.y = 1.0f / ViewportDesc.Height;

	m_iKernelSize = 20;

	_float fCenterWeight = 0.2f;
	_float fAtt = 0.019f;
	m_BlurWeights.resize(m_iKernelSize + 1, fCenterWeight);

	_int iHalf = m_iKernelSize / 2;
	for (_int i = 0; i < m_BlurWeights.size(); ++i)
	{
		_int iWeight = abs(i - iHalf);

		m_BlurWeights[i] = fCenterWeight - (iWeight * fAtt);
	}
}

CRenderer * CRenderer::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CRenderer*	pInstance = new CRenderer(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CRenderer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent * CRenderer::Clone(CGameObject* pObject, void * pArg)
{
	AddRef();

	return this;
}

void CRenderer::Free()
{
	__super::Free();


	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pLight_Manager);

}
