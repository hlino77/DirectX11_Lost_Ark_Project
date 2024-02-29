#include "..\Public\Renderer.h"
#include "GameObject.h"
#include "Shader.h"
#include "GameInstance.h"
#include "NavigationMgr.h"
#include "Target_Manager.h"
#include "Light_Manager.h"
#include "PipeLine.h"
#include "Texture.h"
#include "Graphic_Device.h"
#include "Input_Device.h"
#include "Utils.h"
#include "RenderTarget.h"
#include <string>


_uint CRenderer::m_iIBLTextureIndex = 0;
_uint CRenderer::m_iSSRLevel = 0;
_bool CRenderer::m_bPBR_Switch = true;
_int CRenderer::m_iSSAO_Switch = 0;
_int  CRenderer::m_iFxaa_Switch = true;
_bool CRenderer::m_bDOF_Switch = true;

CRenderer::ScreenTone_Data CRenderer::m_tScreenTone_Data = { 0.5f, 0.5f, 0.5f };
CRenderer::DOF_Data CRenderer::m_tDOF_Data = { 50.f, 7.5f / 1200.f };

_float CRenderer::m_fFogDensity      = 0.f;
_float CRenderer::m_fFogStartHeight  = 0.f;
_float CRenderer::m_fFogEndHeight    = 0.f;
_float CRenderer::m_fFogTime		 = 0.f;
_float CRenderer::m_fFogChangeSpeed  = 0.f;
_float CRenderer::m_fFogMinValue     = 0.f;
Vec3   CRenderer::m_vFogColor        = Vec3(0.f, 0.f, 0.f);

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
	//
	Ready_CascadeShadow();

	//RenderTarget

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_fShadowTargetSizeRatio = 5.12f;
	m_fStaticShadowTargetSizeRatio = 5.12f;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Priority"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Diffuse"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline_Blur_H"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline_Blur_V"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Outline_Blend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_MakeSRV"),
		500, 500, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(1.f, 1.f, 1.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_TextBox"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Normal"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Shade"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Properties"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Emissive"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectOneBlend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectAlphaBlend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_EffectEmissive"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DecalOneBlend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DecalAlphaBlend"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DecalEmissive"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Specular"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_NormalDepth"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(0.0f, 0.0f, -1.0f, 1e5f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_HBAO+"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_B8G8R8A8_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO_Blur_H"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSAO_Blur_HV"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(1.f, 1.f, 1.f, 1.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ShadowDepth"),
		ViewportDesc.Width * m_fShadowTargetSizeRatio, ViewportDesc.Height * m_fShadowTargetSizeRatio, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_StaticShadowDepth"),
	//	ViewportDesc.Width * m_fStaticShadowTargetSizeRatio, ViewportDesc.Height * m_fStaticShadowTargetSizeRatio, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
	//	return E_FAIL;

	//Cascade
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Cascade1"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Cascade2"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Cascade3"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R32G32B32A32_FLOAT, Vec4(1.0f, 1.0f, 1.0f, 1.0f))))
		return E_FAIL;

	// Bloom
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomDownSample1"),
		ViewportDesc.Width / m_fSampleRatio5x5, ViewportDesc.Height / m_fSampleRatio5x5, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomDownSample2"),
		ViewportDesc.Width / m_fSampleRatio25x25, ViewportDesc.Height / m_fSampleRatio25x25, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomDownSample3"),
		ViewportDesc.Width / m_fSampleRatio125x125, ViewportDesc.Height / m_fSampleRatio125x125, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomUpSample1"),
		ViewportDesc.Width / m_fSampleRatio25x25, ViewportDesc.Height / m_fSampleRatio25x25, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomUpSample2"),
		ViewportDesc.Width / m_fSampleRatio5x5, ViewportDesc.Height / m_fSampleRatio5x5, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomUpSample3"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_H_5x5"),
		ViewportDesc.Width / m_fSampleRatio5x5, ViewportDesc.Height / m_fSampleRatio5x5, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_HV_5x5"),
		ViewportDesc.Width / m_fSampleRatio5x5, ViewportDesc.Height / m_fSampleRatio5x5, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_H_25x25"),
		ViewportDesc.Width / m_fSampleRatio25x25, ViewportDesc.Height / m_fSampleRatio25x25, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_HV_25x25"),
		ViewportDesc.Width / m_fSampleRatio25x25, ViewportDesc.Height / m_fSampleRatio25x25, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_H_125x125"),
		ViewportDesc.Width / m_fSampleRatio125x125, ViewportDesc.Height / m_fSampleRatio125x125, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BloomBlur_HV_125x125"),
		ViewportDesc.Width / m_fSampleRatio125x125, ViewportDesc.Height / m_fSampleRatio125x125, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSR"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSRBlur_H"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_SSRBlur_HV"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;*/

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_BlendEffect"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_ChromaticAberration"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_MotionBlur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_RadialBlur"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_PrePostProcess"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Distortion"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DOF_Blur_H"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DOF_Blur_HV"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_DOF_Blended"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_FinalProcessed"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_Dead"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Add_RenderTarget(m_pDevice, m_pContext, TEXT("Target_FXAA"),
		ViewportDesc.Width, ViewportDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, Vec4(0.f, 0.f, 0.f, 0.f))))
		return E_FAIL;*/

//#ifdef _DEBUG
	constexpr _float fTargetX = 128.f;
	constexpr _float fTargetY = 72.f;
	constexpr _float fTargetCX = 256.f;
	constexpr _float fTargetCY = 144.f;

	//Test
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Cascade1"), fTargetX, fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Cascade2"), 3.f * fTargetX, fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Cascade3"), 5.f * fTargetX, fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Normal"), fTargetX, fTargetY, fTargetCX, fTargetCY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Shade"), 3.f * fTargetX, fTargetY, fTargetCX, fTargetCY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_NormalDepth"), 5.f * fTargetX, fTargetY, fTargetCX, fTargetCY)))
		return E_FAIL;*/
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Properties"), 7.f * fTargetX, fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectOneBlend"), fTargetX, 3.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Emissive"), 3.f * fTargetX, 3.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_EffectAlphaBlend"), 5.f * fTargetX, 3.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSAO_Blur_HV"), 7.f * fTargetX, 3.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BloomDownSample1"), fTargetX, 5.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_PrePostProcess"), 3.f * fTargetX, 5.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;	
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_DecalAlphaBlend"), 5.f * fTargetX, 5.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_DecalOneBlend"), 7.f * fTargetX, 5.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_DecalEmissive"), fTargetX, 7.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Distortion"), 3.f * fTargetX, 7.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_SSR"), 5.f * fTargetX, 7.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_BloomBlur_HV_5x5"), 7.f * fTargetX, 7.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline"), fTargetX, 9.f * fTargetY, fTargetCX, fTargetCY)))
	//	return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_Outline_Blend"), 5.f * fTargetX, 1.f * fTargetY, 2.f * fTargetCX, 2.f * fTargetCY)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Ready_Debug(TEXT("Target_DOF_Blur_HV"), 5.f * fTargetX, 5.f * fTargetY, 2.f * fTargetCX, 2.f * fTargetCY)))
		return E_FAIL;

//#endif

	/* 이 렌더타겟들은 그려지는 객체로부터 값을 저장받는다. */
	/* For.MRT_GameObjects */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Priority"), TEXT("Target_Priority"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineObjects"), TEXT("Target_Outline"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineObjects_Blur_H"), TEXT("Target_Outline_Blur_H"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineObjects_Blur_V"), TEXT("Target_Outline_Blur_V"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_OutlineObjects_Blend"), TEXT("Target_Outline_Blend"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Diffuse"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Normal"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_NormalDepth"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Properties"))))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Roughness"))))
		return E_FAIL;*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Emissive"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_GameObjects"), TEXT("Target_Outline"))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_EffectOneBlend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_EffectAlphaBlend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_EffectEmissive"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Effects"), TEXT("Target_Distortion"))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Decals"), TEXT("Target_DecalOneBlend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Decals"), TEXT("Target_DecalAlphaBlend"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Decals"), TEXT("Target_DecalEmissive"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO"), TEXT("Target_SSAO"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO_Blur_H"), TEXT("Target_SSAO_Blur_H"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSAO_Blur_V"), TEXT("Target_SSAO_Blur_HV"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomDownSample1"), TEXT("Target_BloomDownSample1"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomDownSample2"), TEXT("Target_BloomDownSample2"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomDownSample3"), TEXT("Target_BloomDownSample3"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomUpSample1"), TEXT("Target_BloomUpSample1"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomUpSample2"), TEXT("Target_BloomUpSample2"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomUpSample3"), TEXT("Target_BloomUpSample3"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_H_125x125"), TEXT("Target_BloomBlur_H_125x125"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_HV_125x125"), TEXT("Target_BloomBlur_HV_125x125"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_H_25x25"), TEXT("Target_BloomBlur_H_25x25"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_HV_25x25"), TEXT("Target_BloomBlur_HV_25x25"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_H_5x5"), TEXT("Target_BloomBlur_H_5x5"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BloomBlur_HV_5x5"), TEXT("Target_BloomBlur_HV_5x5"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSR"), TEXT("Target_SSR"))))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSRBlur_H"), TEXT("Target_SSRBlur_H"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_SSRBlur_HV"), TEXT("Target_SSRBlur_HV"))))
		return E_FAIL;*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DOF_Blur_H"), TEXT("Target_DOF_Blur_H"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DOF_Blur_HV"), TEXT("Target_DOF_Blur_HV"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DepthOfField"), TEXT("Target_DOF_Blended"))))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_PrePostProcessScene"), TEXT("Target_PrePostProcess"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_FinalProcessedScene"), TEXT("Target_FinalProcessed"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_DeadScene"), TEXT("Target_Dead"))))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_FXAA"), TEXT("Target_FXAA"))))
		return E_FAIL;*/

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_BlendEffect"), TEXT("Target_BlendEffect"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ChromaticAberration"), TEXT("Target_ChromaticAberration"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_MotionBlur"), TEXT("Target_MotionBlur"))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_RadialBlur"), TEXT("Target_RadialBlur"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_ShadowDepth"), TEXT("Target_ShadowDepth"))))
		return E_FAIL;
	/*if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_StaticShadowDepth"), TEXT("Target_StaticShadowDepth"))))
		return E_FAIL;*/
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_MakeSRV"), TEXT("Target_MakeSRV"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_TextBox"), TEXT("Target_TextBox"))))
		return E_FAIL;

	/* 이 렌더타겟들은 게임내에 존재하는 빛으로부터 연산한 결과를 저장받는다. */
	/* For.MRT_ */
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Lights"), TEXT("Target_Shade"))))
		return E_FAIL;

	//cascade
	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Cascade1"), TEXT("Target_Cascade1"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Cascade2"), TEXT("Target_Cascade2"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Add_MRT(TEXT("MRT_Cascade3"), TEXT("Target_Cascade3"))))
		return E_FAIL;
	//

	m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
	
	m_pVIBufferSSAO = CVIBuffer_RectSSAO::Create(m_pDevice, m_pContext);
	if (nullptr == m_pVIBufferSSAO)
		return E_FAIL;

	m_pMRTShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pMRTShader)
		return E_FAIL;
	if (FAILED(m_pMRTShader->Initialize()))
		return E_FAIL;
	
	m_pSSRShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SSR.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pSSRShader)
		return E_FAIL;
	if (FAILED(m_pSSRShader->Initialize()))
		return E_FAIL;
	
	m_pBloomShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Bloom.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pBloomShader)
		return E_FAIL;
	if (FAILED(m_pBloomShader->Initialize()))
		return E_FAIL;

	m_pPostProccessor = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_PostProcess.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pPostProccessor)
		return E_FAIL;
	if (FAILED(m_pPostProccessor->Initialize()))
		return E_FAIL;

	m_pFxaaShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Fxaa3_11.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pFxaaShader)
		return E_FAIL;
	if (FAILED(m_pFxaaShader->Initialize()))
		return E_FAIL;

	m_pOutlineBlurShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Outline_Blur.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pOutlineBlurShader)
		return E_FAIL;
	if (FAILED(m_pOutlineBlurShader->Initialize()))
		return E_FAIL;

	if (FAILED(Ready_MakeSRV_DSV()))
		return E_FAIL;
	if (FAILED(Ready_ShadowDSV()))
		return E_FAIL;

	m_WorldMatrix = XMMatrixIdentity();
	m_WorldMatrix._11 = ViewportDesc.Width;
	m_WorldMatrix._22 = ViewportDesc.Height;

	m_ViewMatrix = XMMatrixIdentity();
	
	m_ProjMatrix = XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f);

	//DOF
	if (FAILED(Ready_DOF()))
		return E_FAIL;

	//Blur
	if (FAILED(Ready_BlurData()))
		return E_FAIL;

	//SSAO
	if (FAILED(Ready_SSAO()))
		return E_FAIL;
	//IBL
	if (FAILED(Ready_IBL()))
		return E_FAIL;

	//SSR
	if (FAILED(Ready_SSR()))
		return E_FAIL;

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

	m_InstanceRenderObjects[eRenderGroup][pGameObject->Get_ModelName()].clear();

	return S_OK;
}

HRESULT CRenderer::Add_RenderGroup(RENDERGROUP eRenderGroup, CGameObject * pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_RenderObjects[eRenderGroup].push_back(pGameObject);

	Safe_AddRef(pGameObject);

	return S_OK;
}

HRESULT CRenderer::Add_InstanceRenderGroup(RENDERGROUP eRenderGroup, CGameObject* pGameObject)
{
	if (eRenderGroup >= RENDER_END)
		return E_FAIL;

	m_InstanceRenderObjects[eRenderGroup][pGameObject->Get_ModelName()].push_back(pGameObject);

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

HRESULT CRenderer::Add_DebugObject(CGameObject* pObject)
{
	m_DebugRenderObjects.push_back(pObject);
	Safe_AddRef(pObject);
	return S_OK;
}

HRESULT CRenderer::Add_CascadeInstanceGroup(CGameObject* pGameObject)
{
	m_InstanceCascadeObjects[pGameObject->Get_ModelName()].push_back(pGameObject);
	Safe_AddRef(pGameObject);
	return S_OK;
}

HRESULT CRenderer::Add_CascadeObject(_uint iIndex, CGameObject* pObject)
{
	m_CascadeObjects[iIndex].push_back(pObject);
	Safe_AddRef(pObject);
	return S_OK;
}

HRESULT CRenderer::Ready_InstanceRender()
{
	for (_uint i = 0; i < CRenderer::RENDER_END; ++i)
	{
		if (i == RENDER_SHADOW)
			continue;

		if (!m_InstanceRenderObjects[i].empty())
		{
			for (auto& ObjectList : m_InstanceRenderObjects[i])
			{
				if (!ObjectList.second.empty())
				{
					_uint iIndex = 0;
					for (auto& Object : ObjectList.second)
					{
						Object->Add_InstanceData((_uint)ObjectList.second.size(), iIndex);
					}
				}
			}
		}
		
	}

	
	for (auto& List : m_InstanceCascadeObjects)
	{
		if (List.second.empty() == false)
		{
			_uint iIndex = 0;
			for (auto& Object : List.second)
			{
				Object->Add_Cascade_InstanceData(List.second.size(), iIndex);
			}
		}
	}

	return S_OK;
}

HRESULT CRenderer::Draw()
{
	//CGameInstance::GetInstance()->Execute_BeforeRenderCommandList();
	/*if (m_bRenderStaticShadow)
		if (FAILED(Render_StaticShadow()))
			return E_FAIL;*/

	if (FAILED(Update_TextBox()))
		return E_FAIL;
	if (FAILED(Render_MakeSRV()))
		return E_FAIL;
	if (FAILED(Render_Priority()))
		return E_FAIL;

	if (FAILED(Render_NonAlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_Cascade()))
		return E_FAIL;

	if (0 == m_iSSAO_Switch)
	{
		if (FAILED(Render_HBAOPLUS()))
			return E_FAIL;
	}
	else if (1 == m_iSSAO_Switch)
	{
		if (FAILED(Render_SSAO()))
			return E_FAIL;
	}

	if (FAILED(Render_Decal()))
		return E_FAIL;
	if (FAILED(Render_Effect()))
		return E_FAIL;
	if (FAILED(Render_ShadowDepth()))
		return E_FAIL;
	if (FAILED(Render_Lights()))
		return E_FAIL; 

	if (FAILED(Render_Outline()))
		return E_FAIL;

	if (FAILED(Render_Bloom()))
		return E_FAIL;

	if (FAILED(Render_Deferred()))
		return E_FAIL;

	if (FAILED(Render_SSR()))
		return E_FAIL;
	if (FAILED(Render_Blend()))
		return E_FAIL;
	if (FAILED(Render_NonLight()))
		return E_FAIL;

	if (FAILED(Render_PostProcess()))
		return E_FAIL;

	if (FAILED(Render_FXAA()))
		return E_FAIL;

	// ...


	if (FAILED(Render_AlphaBlend()))
		return E_FAIL;

	if (FAILED(Render_WorldUI()))
		return E_FAIL;

	if (FAILED(Render_UI()))
		return E_FAIL;

	if (FAILED(Render_Esther()))
		return E_FAIL;

	if (FAILED(Render_Mouse()))
		return E_FAIL;


	if (FAILED(Render_DebugObject()))
		return E_FAIL;


	if (KEY_HOLD(KEY::CTRL) && KEY_HOLD(KEY::SHIFT) && KEY_TAP(KEY::T))
		m_bTargetOnOff = !m_bTargetOnOff;

	if (m_bTargetOnOff)
		if (FAILED(Render_Debug()))
			return E_FAIL;

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

HRESULT CRenderer::Bind_TextBoxSRV(CShader* pShader)
{
	if (FAILED(m_pTarget_Manager->Bind_SRV(pShader, TEXT("Target_TextBox"), "g_TextBoxTexture")))
		return E_FAIL;

	return S_OK;
}

void CRenderer::Set_RadialBlurData(Vec3 vWorldPos, _float fIntensity)
{
	m_tRadialBlur_Data.vRadialBlurWorldPos = vWorldPos;
	m_tRadialBlur_Data.fRadialBlurIntensity = fIntensity;
}

void CRenderer::Set_ChromaticData(Vec3 vWorldPos, _float fIntensity)
{
	m_tChromatic_Data.vChromaticWorldPos = vWorldPos;
	m_tChromatic_Data.fChromaticIntensity = fIntensity;
}

HRESULT CRenderer::Update_TextBox()
{
	CGameInstance::GetInstance()->JobMutex_Lock();

	for (auto& iter : m_RenderObjects[UPDATE_TEXTBOX])
	{
		if (FAILED(iter->Render_MakeSRV()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[UPDATE_TEXTBOX].clear();

	CGameInstance::GetInstance()->JobMutex_UnLock();

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

	ViewportDesc.Width = fOriginalWidth;
	ViewportDesc.Height = fOriginalHeight;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);

	return S_OK;
}

HRESULT CRenderer::Render_Priority()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Priority"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_PRIORITY])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_PRIORITY].clear();

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


	for (auto& ObjectList : m_InstanceRenderObjects[RENDER_SHADOW])
	{
		if (!ObjectList.second.empty())
		{
			if (FAILED(ObjectList.second[0]->Render_ShadowDepth_Instance((_uint)ObjectList.second.size())))
				return E_FAIL;

			for (auto& Object : ObjectList.second)
				Safe_Release(Object);

			ObjectList.second.clear();
		}
	}



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

	m_pTarget_Manager->Make_SRVTexture(L"../Bin/Resources/Textures/LightMap/LightMap1.dds", L"Target_StaticShadowDepth");

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

		Safe_Release(iter);
	}

	m_RenderObjects[RENDER_NONBLEND].clear();

	for (auto& ObjectList : m_InstanceRenderObjects[RENDER_NONBLEND])
	{
		if (!ObjectList.second.empty())
		{
			if (FAILED(ObjectList.second[0]->Render_Instance((_uint)ObjectList.second.size())))
				return E_FAIL;

			for (auto& Object : ObjectList.second)
				Safe_Release(Object);

			ObjectList.second.clear();
		}
	}

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Outline()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_OutlineObjects"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDER_OUTLINE])
	{
		if (FAILED(iter->Render_Outline()))
			return E_FAIL;

		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_OUTLINE].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Decal()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Decals"))))
		return E_FAIL;

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_DECAL])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_DECAL].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Effect()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effects"))))
		return E_FAIL;

	/*for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_DECAL])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_DECAL].clear();*/

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_EFFECT])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_EFFECT].clear();

	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_EFFECTPARTICLE])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_EFFECTPARTICLE].clear();

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

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

HRESULT CRenderer::Render_SSR()
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSR"))))
		return E_FAIL;

	if (FAILED(m_pSSRShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSRShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSRShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_PrePostProcess"), "g_PrePostProcessTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_Normal"), "g_NormalTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_Properties"), "g_PropertiesTarget")))
		return E_FAIL;

#ifdef _DEBUG
	if (FAILED(m_pSSRShader->Bind_CBuffer("SSR_Data", &m_tSSR_Data[0], sizeof(SSR_Data))))
		return E_FAIL;
#else
	if (FAILED(m_pSSRShader->Bind_CBuffer("SSR_Data", &m_tSSR_Data[m_iSSRLevel], sizeof(SSR_Data))))
		return E_FAIL;
#endif

	const Vec4& vCamPos = pPipeLine->Get_CamPosition();
	if (FAILED(m_pSSRShader->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(Vec4))))
		return E_FAIL;

	//const Matrix& matCamProjInv = pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ);
	//const Matrix& matCamViewInv = pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);
	//Matrix matCamProjViewInv = matCamProjInv * matCamViewInv;	// TODO : 저장해 두고 쓸 것.

	/*if (FAILED(m_pSSRShader->Bind_Matrix("g_ProjViewMatrixInv", &matCamProjViewInv)))
		return E_FAIL;*/
	if (FAILED(m_pSSRShader->Bind_Matrix("g_ViewMatrixInv", &pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pSSRShader->Bind_Matrix("g_ProjMatrixInv", &pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	//const Matrix& matCamProj = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	//const Matrix& matCamView = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
	//Matrix matCamProjView = matCamProj * matCamView;	// TODO : 저장해 두고 쓸 것.
	/*if (FAILED(m_pSSRShader->Bind_Matrix("g_CamViewProjMatrix", &matCamProjView)))
		return E_FAIL;*/

	if (FAILED(m_pSSRShader->Bind_Matrix("g_CamViewMatrix", &pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pSSRShader->Bind_Matrix("g_CamProjMatrix", &pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;

	if (FAILED(m_pSSRShader->Begin("SSR")))
		return E_FAIL;
	
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSRBlur_H"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_SSR"), "g_SSRTarget")))
		return E_FAIL;

	if (FAILED(m_pSSRShader->Begin("SSR_Blur_H")))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSRBlur_HV"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSRShader, TEXT("Target_SSRBlur_H"), "g_SSRTarget")))
		return E_FAIL;

	if (FAILED(m_pSSRShader->Begin("SSR_Blur_V")))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;*/

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

HRESULT CRenderer::Render_HBAOPLUS()
{
	GFSDK_SSAO_InputData_D3D11 Input;
	Input.DepthData.DepthTextureType = GFSDK_SSAO_HARDWARE_DEPTHS;
	Input.DepthData.pFullResDepthTextureSRV = CGraphic_Device::GetInstance()->Get_DepthStencilSRV();

	const Matrix* ProjMatrix = &CPipeLine::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

	Input.DepthData.ProjectionMatrix.Data = GFSDK_SSAO_Float4x4((const GFSDK_SSAO_FLOAT*)ProjMatrix);
	Input.DepthData.ProjectionMatrix.Layout = GFSDK_SSAO_ROW_MAJOR_ORDER;
	Input.DepthData.MetersToViewSpaceUnits = 1.f;

	GFSDK_SSAO_Parameters Params;
	Params.Radius = 2.f;
	Params.Bias = 0.1f;
	Params.PowerExponent = 2.f;
	Params.Blur.Enable = true;
	Params.Blur.Radius = GFSDK_SSAO_BLUR_RADIUS_4;
	Params.Blur.Sharpness = 16.f;

	GFSDK_SSAO_Output_D3D11 Output;
	Output.pRenderTargetView = m_pTarget_Manager->Find_RenderTarget(TEXT("Target_HBAO+"))->Get_RTV();
	Output.Blend.Mode = GFSDK_SSAO_OVERWRITE_RGB;

	GFSDK_SSAO_Status status;
	status = CGraphic_Device::GetInstance()->Get_AOContext()->RenderAO(m_pContext, Input, Params, Output);
	assert(status == GFSDK_SSAO_OK);

	return S_OK;
}

HRESULT CRenderer::Render_SSAO()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO"))))
		return E_FAIL;

	XMMATRIX P = m_pGameInstance->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
	XMMATRIX PT = XMMatrixMultiply(P, m_matToTexture);

	m_tSSAO_Data.matViewToTexSpace = PT;
	for (_int i = 0; i < 4; ++i)
		m_tSSAO_Data.vFrustumCorners[i] = m_vFrustumFarCorner[i];

	for (_int i = 0; i < 14; ++i)
		m_tSSAO_Data.vOffsetVectors[i] = m_vOffsets[i];

	if (FAILED(m_pSSAOShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	/*if (FAILED(m_pSSAOShader->Bind_Matrix("g_CamViewMatrix", &pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Bind_Matrix("g_ProjMatrixInv", &pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;*/

	RELEASE_INSTANCE(CPipeLine);

	if (FAILED(m_pSSAOShader->Bind_CBuffer("PerFrame", &m_tSSAO_Data, sizeof(SSAO_Data))))
		return E_FAIL;

	if (FAILED(m_pSSAOShader->Bind_Texture("g_RandomVectorTexture", m_pRandomSRV)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSAOShader, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")))
		return E_FAIL;

	if (FAILED(m_pSSAOShader->Begin("SSAO")))
		return E_FAIL;

	if (FAILED(m_pVIBufferSSAO->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	//////////////////////////////// Blur

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO_Blur_H"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSAOShader, TEXT("Target_SSAO"), "g_SSAOBlurTarget")))
		return E_FAIL;

	if (FAILED(m_pSSAOShader->Begin("SSAO_Blur_H")))
		return E_FAIL;

	if (FAILED(m_pVIBufferSSAO->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO_Blur_V"))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSAOShader, TEXT("Target_SSAO_Blur_H"), "g_SSAOBlurTarget")))
		return E_FAIL;

	if (FAILED(m_pSSAOShader->Begin("SSAO_Blur_V")))
		return E_FAIL;

	if (FAILED(m_pVIBufferSSAO->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	//////////
	/*for (_int i = 0; i < 1; ++i)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO_Blur_H"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSAOShader, TEXT("Target_SSAO_Blur_HV"), "g_SSAOBlurTarget")))
			return E_FAIL;

		if (FAILED(m_pSSAOShader->Begin("SSAO_Blur_H")))
			return E_FAIL;

		if (FAILED(m_pVIBufferSSAO->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_SSAO_Blur_V"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pSSAOShader, TEXT("Target_SSAO_Blur_H"), "g_SSAOBlurTarget")))
			return E_FAIL;

		if (FAILED(m_pSSAOShader->Begin("SSAO_Blur_V")))
			return E_FAIL;

		if (FAILED(m_pVIBufferSSAO->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	}*/

	return S_OK;
}



HRESULT CRenderer::Render_Deferred()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_PrePostProcessScene"))))
		return E_FAIL;

	/* 디퓨즈 타겟과 셰이드 타겟을 서로 곱하여 백버퍼에 최종적으로 찍어낸다. */
	if (FAILED(m_pMRTShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	const Vec4& vCamPos = CPipeLine::GetInstance()->Get_CamPosition();
	if (FAILED(m_pMRTShader->Bind_RawValue("g_vCamPosition", &vCamPos, sizeof(Vec4))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_Matrix("g_LightViewMatrix", &CLight_Manager::GetInstance()->Get_DirectionLightMatrix())))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_LightProjMatrix", &CPipeLine::GetInstance()->Get_TransformMatrix(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrixInv", &CPipeLine::GetInstance()->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW))))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrixInv", &CPipeLine::GetInstance()->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ))))
		return E_FAIL;


	if (FAILED(m_pMRTShader->Bind_Matrix("g_StaticLightViewMatrix", &CLight_Manager::GetInstance()->Get_StaticLightMatrix())))
		return E_FAIL;


	if (FAILED(m_pMRTShader->Bind_RawValue("g_fBias", &m_fBias, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fStaticBias", &m_fStaticBias, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fShadowSizeRatio", &m_fShadowTargetSizeRatio, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fStaticShadowSizeRatio", &m_fStaticShadowTargetSizeRatio, sizeof(_float))))
		return E_FAIL;


	//Test
	if (FAILED(m_pMRTShader->Bind_Matrices("g_CascadeProj", CPipeLine::GetInstance()->Get_ShadowProj(), 3)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Cascade1"), "g_CascadeTarget1")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Cascade2"), "g_CascadeTarget2")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Cascade3"), "g_CascadeTarget3")))
		return E_FAIL;
	//

	// For Fog
	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogStartHeight", &m_fFogStartHeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogEndHeight", &m_fFogEndHeight, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogDensity", &m_fFogDensity, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_vFogColor", &m_vFogColor, sizeof(Vec3))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogTime", &m_fFogTime, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogChangeSpeed", &m_fFogChangeSpeed, sizeof(_float))))
		return E_FAIL;

	if (FAILED(m_pMRTShader->Bind_RawValue("g_fFogMinValue", &m_fFogMinValue, sizeof(_float))))
		return E_FAIL;



	if (FAILED(m_pLight_Manager->Bind_LightDescription(m_pMRTShader)))
		return E_FAIL;
	//if (FAILED(m_pLight_Manager->Bind_LightShadowTexture(m_pMRTShader)))
	//	return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_ShadowDepth"), "g_ShadowDepthTarget")))
		return E_FAIL;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Priority"), "g_PriorityTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Diffuse"), "g_DiffuseTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Normal"), "g_NormalTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Emissive"), "g_EmissiveTarget")))
		return E_FAIL;

	/*if (FAILED(m_pMRTShader->Bind_RawValue("g_bSSAO", &m_iSSAO_Switch, sizeof(_int))))
		return E_FAIL;

	if(1 == m_iSSAO_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_SSAO_Blur_HV"), "g_SSAOBlurTarget")))
			return E_FAIL;
	}
	else if (2 == m_iSSAO_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_HBAO+"), "g_SSAOBlurTarget")))
			return E_FAIL;
	}*/

	//
	if (1 == m_iSSAO_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_SSAO_Blur_HV"), "g_SSAOBlurTarget")))
			return E_FAIL;
	}
	else if (0 == m_iSSAO_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_HBAO+"), "g_SSAOBlurTarget")))
			return E_FAIL;
	}
	//

	if (m_bPBR_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pMRTShader, TEXT("Target_Properties"), "g_PropertiesTarget")))
			return E_FAIL;

		if (FAILED(m_pMRTShader->Bind_Texture("g_IrradianceTexture", m_pIrradianceTexture->Get_SRV(m_iIBLTextureIndex))) ||
			FAILED(m_pMRTShader->Bind_Texture("g_PreFilteredTexture", m_pPreFilteredTexture->Get_SRV(m_iIBLTextureIndex))) ||
			FAILED(m_pMRTShader->Bind_Texture("g_BRDFTexture", m_pBRDFTexture->Get_SRV())))
			return E_FAIL;

		if (FAILED(m_pMRTShader->Begin("PBR_Deferred")))
			return E_FAIL;
	}
	else
	{/* Shade, Specular 삭제 */
		if (FAILED(m_pMRTShader->Begin("Deferred")))
			return E_FAIL;
	}

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_Blend()
{
	//if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_Effects"))))
	//	return E_FAIL;

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

HRESULT CRenderer::Render_Bloom()
{
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomDownSample1"), m_pDownSample5x5_DSV)))
		return E_FAIL;

	//m_pContext->ClearDepthStencilView(m_pDownSample5x5_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	if (FAILED(m_pBloomShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)) ||
		FAILED(m_pBloomShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)) ||
		FAILED(m_pBloomShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	_float fOriginalWidth = ViewportDesc.Width;
	_float fOriginalHeight = ViewportDesc.Height;

	ViewportDesc.Width = fOriginalWidth / m_fSampleRatio5x5;
	ViewportDesc.Height = fOriginalHeight / m_fSampleRatio5x5;
	
	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_Emissive"), "g_BloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_DecalEmissive"), "g_DecalBloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_EffectEmissive"), "g_EffectBloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_Outline"), "g_OulineBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("DownSample_Bright")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 1

	ViewportDesc.Width = fOriginalWidth / m_fSampleRatio25x25;
	ViewportDesc.Height = fOriginalHeight / m_fSampleRatio25x25;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomDownSample2"), m_pDownSample25x25_DSV)))
		return E_FAIL;
	    
	//m_pContext->ClearDepthStencilView(m_pDownSample25x25_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomDownSample1"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("DownSample")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 2

	ViewportDesc.Width = fOriginalWidth / m_fSampleRatio125x125;
	ViewportDesc.Height = fOriginalHeight / m_fSampleRatio125x125;

	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomDownSample3"), m_pDownSample125x125_DSV)))
		return E_FAIL;

	//m_pContext->ClearDepthStencilView(m_pDownSample125x125_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomDownSample2"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("DownSample")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 3

	tagPerFrame tBloomData = { m_fSampleRatio125x125 / fOriginalWidth, m_fSampleRatio125x125 / fOriginalHeight };
	if (FAILED(m_pBloomShader->Bind_CBuffer("PerFrame", &tBloomData, sizeof(tagPerFrame))))
		return E_FAIL;
	// 144x144에서 블러
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_H_125x125"), m_pDownSample125x125_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomDownSample3"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurH")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_HV_125x125"), m_pDownSample125x125_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_H_125x125"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurV")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 4
	
	// 여기부터 UpSample

	ViewportDesc.Width = fOriginalWidth / m_fSampleRatio25x25;
	ViewportDesc.Height = fOriginalHeight / m_fSampleRatio25x25;
	// 6x6 Up
	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomUpSample1"), m_pDownSample25x25_DSV)))
		return E_FAIL;

	// Target_BloomBlur_HV 얘가 날아가는건지 안뜸.
	//m_pContext->ClearDepthStencilView(m_pDownSample25x25_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomDownSample2"), "g_BloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_HV_125x125"), "g_DownSampledTarget")) ||
		FAILED(m_pBloomShader->Begin("UpSample")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 5


	// 24x24에서 블러
	tBloomData = { m_fSampleRatio25x25 / fOriginalWidth, m_fSampleRatio25x25 / fOriginalHeight };
	if (FAILED(m_pBloomShader->Bind_CBuffer("PerFrame", &tBloomData, sizeof(tagPerFrame))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_H_25x25"), m_pDownSample25x25_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomUpSample1"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurH")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_HV_25x25"), m_pDownSample25x25_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_H_25x25"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurV")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 6

	ViewportDesc.Width = fOriginalWidth / m_fSampleRatio5x5;
	ViewportDesc.Height = fOriginalHeight / m_fSampleRatio5x5;
	// 6x6 up
	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomUpSample2"), m_pDownSample5x5_DSV)))
		return E_FAIL;

	//m_pContext->ClearDepthStencilView(m_pDownSample5x5_DSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomDownSample1"), "g_BloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_HV_25x25"), "g_DownSampledTarget")) ||
		FAILED(m_pBloomShader->Begin("UpSample")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	//7

	// 4x4에서 블러
	tBloomData = { m_fSampleRatio5x5 / fOriginalWidth, m_fSampleRatio5x5 / fOriginalHeight };
	if (FAILED(m_pBloomShader->Bind_CBuffer("PerFrame", &tBloomData, sizeof(tagPerFrame))))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_H_5x5"), m_pDownSample5x5_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomUpSample2"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurH")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomBlur_HV_5x5"), m_pDownSample5x5_DSV)) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_H_5x5"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("BloomBlurV")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;
	// 8

	ViewportDesc.Width = fOriginalWidth;
	ViewportDesc.Height = fOriginalHeight;
	// 4x4 up
	m_pContext->RSSetViewports(iNumViewports, &ViewportDesc);
	/*if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BloomUpSample3"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_Emissive"), "g_BloomBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pBloomShader, TEXT("Target_BloomBlur_HV"), "g_BloomBlurTarget")) ||
		FAILED(m_pBloomShader->Begin("UpSample")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;*/
	// 9

	return S_OK;
}

HRESULT CRenderer::Render_DepthOfField()
{
	if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::D))
		m_bDOF_Switch = !m_bDOF_Switch;

	if (true == m_bDOF_Switch)
	{
		//m_tDOF_Data.fRange = 20.f;
		//m_tDOF_Data.fFocus = 0.00625f; // 7.5f / 1200.f;
	}
	else if (false == m_bDOF_Switch)
	{
		return S_OK;
	}

	if (FAILED(m_pDOFShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)) ||
		FAILED(m_pDOFShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)) ||
		FAILED(m_pDOFShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_DOF_Blur_H"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pDOFShader, TEXT("Target_Outline_Blend"), "g_DOFBlurTarget")) ||
		FAILED(m_pDOFShader->Begin("DOFBlurH")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	//
	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_DOF_Blur_HV"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pDOFShader, TEXT("Target_DOF_Blur_H"), "g_DOFBlurTarget")) ||
		FAILED(m_pDOFShader->Begin("DOFBlurV")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	//
	if (FAILED(m_pDOFShader->Bind_CBuffer("DOFData", &m_tDOF_Data, sizeof(DOF_Data))))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_DepthOfField"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pDOFShader, TEXT("Target_DOF_Blur_HV"), "g_DOFBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pDOFShader, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pDOFShader, TEXT("Target_Outline_Blend"), "g_PreProcessedTarget")) ||
		FAILED(m_pDOFShader->Begin("DepthOfField")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_OutlineBlur()
{
	if (FAILED(m_pOutlineBlurShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)) ||
		FAILED(m_pOutlineBlurShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)) ||
		FAILED(m_pOutlineBlurShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;


	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_OutlineObjects_Blur_H"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pOutlineBlurShader, TEXT("Target_Outline"), "g_OutBlurTarget")) ||
		FAILED(m_pOutlineBlurShader->Begin("BlurH")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_OutlineObjects_Blur_V"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pOutlineBlurShader, TEXT("Target_Outline_Blur_H"), "g_OutBlurTarget")) ||
		FAILED(m_pOutlineBlurShader->Begin("BlurV")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_OutlineObjects_Blend"))) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_Outline_Blur_V"), "g_OutlineBlurTarget")) ||
		FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_SSR"), "g_PrePostProcessTarget")) ||
		FAILED(m_pPostProccessor->Begin("OutlineBlur")) || FAILED(m_pVIBuffer->Render()) ||
		FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Render_PostProcess()
{
	CPipeLine* pPipeLine = GET_INSTANCE(CPipeLine);

	if (FAILED(Render_OutlineBlur()))
		return E_FAIL;

	if (FAILED(Render_DepthOfField()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_BlendEffect"))))
		return E_FAIL;
	
	if (FAILED(m_pPostProccessor->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pPostProccessor->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pPostProccessor->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pPostProccessor->Begin("BlendEffect")))
		return E_FAIL;

	if (true == m_bDOF_Switch)
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_DOF_Blended"), "g_SSRTarget")))
			return E_FAIL;	// 임시
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_DOF_Blended"), "g_PrePostProcessTarget")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_SSR"), "g_SSRTarget")))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_Outline_Blend"), "g_PrePostProcessTarget")))
			return E_FAIL;
	}
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_DecalOneBlend"), "g_DecalOneBlendTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_DecalAlphaBlend"), "g_DecalAlphaBlendTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_EffectOneBlend"), "g_EffectOneBlendTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_EffectAlphaBlend"), "g_EffectAlphaBlendTarget")))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_Distortion"), "g_DistortionTarget")))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_BloomBlur_HV_5x5"), "g_BloomTarget")))
		return E_FAIL;
	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_BlendEffect"), "g_ProcessingTarget")))
		return E_FAIL;

	if (FLT_EPSILON < m_tChromatic_Data.fChromaticIntensity)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_ChromaticAberration"))))
			return E_FAIL;
		
		if (FAILED(m_pPostProccessor->Begin("ChromaticAberration")))
			return E_FAIL;

		Matrix matCamView = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);
		Matrix matCamProj = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);

		if (FAILED(m_pPostProccessor->Bind_Matrix("g_CamViewMatrix", &matCamView)))
			return E_FAIL;
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_CamProjMatrix", &matCamProj)))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Bind_CBuffer("ChromaticBlur", &m_tChromatic_Data, sizeof(ChromaticBlur_Data))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_BlendEffect"), "g_ProcessingTarget")))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_ChromaticAberration"), "g_ProcessingTarget")))
			return E_FAIL;
	}

	// Motion Blur
	if (pPipeLine->Is_MotionBlur() || false == m_bMotionBlurInitialized)
	{
		if (false == m_bMotionBlurInitialized) m_bMotionBlurInitialized = true;

		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_MotionBlur"))))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Begin("MotionBlur")))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_Normal"), "g_NormalTarget")))
			return E_FAIL;
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_NormalDepth"), "g_NormalDepthTarget")))
			return E_FAIL;

		const Matrix& matCamProjInv = pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_PROJ);
		const Matrix& matCamViewInv = pPipeLine->Get_TransformMatrixInverse(CPipeLine::D3DTS_VIEW);
		//Matrix matCamProjViewInv = matCamProjInv * matCamViewInv;

		/*if (FAILED(m_pPostProccessor->Bind_Matrix("g_ProjViewMatrixInv", &matCamProjViewInv)))
			return E_FAIL;*/
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_ProjMatrixInv", &matCamProjInv)))
			return E_FAIL;
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_ViewMatrixInv", &matCamViewInv)))
			return E_FAIL;
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_PreCamViewMatrix", &m_matPreCamView)))
			return E_FAIL;

		Matrix matCamProj = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_CamProjMatrix", &matCamProj)))
			return E_FAIL;

		_float fIntensity = CPipeLine::GetInstance()->Get_MotionBlurIntensity();
		if (FAILED(m_pPostProccessor->Bind_RawValue("g_fMotionBlurIntensity", &fIntensity, sizeof(_float))))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_MotionBlur"), "g_ProcessingTarget")))
			return E_FAIL;
	}

	m_matPreCamView = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_VIEW);

	if (FLT_EPSILON < m_tRadialBlur_Data.fRadialBlurIntensity)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_RadialBlur"))))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Begin("RadialBlur")))
			return E_FAIL;
		if (FAILED(m_pPostProccessor->Bind_CBuffer("RadialBlur", &m_tRadialBlur_Data, sizeof(RadialBlur_Data))))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Bind_Matrix("g_CamViewMatrix", &m_matPreCamView)))
			return E_FAIL;

		Matrix matCamProj = pPipeLine->Get_TransformMatrix(CPipeLine::D3DTS_PROJ);
		if (FAILED(m_pPostProccessor->Bind_Matrix("g_CamProjMatrix", &matCamProj)))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_RadialBlur"), "g_ProcessingTarget")))
			return E_FAIL;

		m_tRadialBlur_Data.fRadialBlurIntensity = 0.f;
		m_tRadialBlur_Data.vRadialBlurWorldPos = Vec3(0.f, 0.f, 0.f);
	}

	// 최종 화면
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_FinalProcessedScene"))))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Begin("PostProcess")))
			return E_FAIL;
		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	
	}

	// 데드신
	//if(true == m_bDeadScene)
	{
		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_DeadScene"))))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_FinalProcessed"), "g_FinalScene")))
			return E_FAIL;

		if (FAILED(m_pPostProccessor->Begin("ScreenTone")))
			return E_FAIL;

		if (true == m_bDeadScene)
		{
			if (false == m_bDeadSceneStart)
			{
				m_tScreenTone_BackUp = m_tScreenTone_Data;
				m_bDeadSceneStart = true;
			}

			m_tScreenTone_Data.fGrayScale = 0.1f;
			m_tScreenTone_Data.fContrast = 0.5f;
			m_tScreenTone_Data.fSaturation = 0.5f;
		}
		else
		{
			if (true == m_bDeadSceneStart)
			{
				m_tScreenTone_Data = m_tScreenTone_BackUp;

				m_bDeadSceneStart = false;
			}
		}

		if (FAILED(m_pPostProccessor->Bind_CBuffer("ScreenTone", &m_tScreenTone_Data, sizeof(ScreenTone_Data))))
			return E_FAIL;

		if (FAILED(m_pVIBuffer->Render()))
			return E_FAIL;

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;
	}

	/*if (FAILED(m_pTarget_Manager->Bind_SRV(m_pPostProccessor, TEXT("Target_BlendBloom"), "g_BlendedTarget")))
		return E_FAIL;*/


	if (true == m_bScreenShot)
	{
		wstring strFinalPath = m_strScreenShotPath + to_wstring(m_iScreenShotFrame) + TEXT(".dds");
		m_pTarget_Manager->Make_SRVTexture(strFinalPath, TEXT("Target_FinalProcessed"));
		m_iScreenShotFrame++;
	}

	RELEASE_INSTANCE(CPipeLine);

	return S_OK;
}

HRESULT CRenderer::Render_FXAA()
{
	/*if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, TEXT("MRT_FXAA"))))
		return E_FAIL;*/

	if (FAILED(m_pFxaaShader->Bind_Matrix("g_WorldMatrix", &m_WorldMatrix)))
		return E_FAIL;
	if (FAILED(m_pFxaaShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pFxaaShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	_int a = false;
	if (FAILED(m_pFxaaShader->Bind_RawValue("g_bFxaa", &m_iFxaa_Switch, sizeof(_int))))
		return E_FAIL;

	if (false == m_bDeadScene)
	{
		//if (FAILED(m_pTarget_Manager->Bind_SRV(m_pFxaaShader, TEXT("Target_FinalProcessed"), "g_FinalTarget")))
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pFxaaShader, TEXT("Target_Dead"), "g_FinalTarget")))
			return E_FAIL;
	}
	else
	{
		if (FAILED(m_pTarget_Manager->Bind_SRV(m_pFxaaShader, TEXT("Target_Dead"), "g_FinalTarget")))
			return E_FAIL;
	}

	if (FAILED(m_pFxaaShader->Begin("Fxaa3_11")))
		return E_FAIL;

	if (FAILED(m_pVIBuffer->Render()))
		return E_FAIL;

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

HRESULT CRenderer::Render_Esther()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_ESHTER])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_ESHTER].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Cascade()
{
	for (_uint i = 0; i < 3; ++i)
	{
		wstring szTargetName = L"MRT_Cascade";
		szTargetName += to_wstring(i + 1);

		if (FAILED(m_pTarget_Manager->Begin_MRT(m_pContext, szTargetName, m_pCascadeShadowDSV[i])))
			return E_FAIL;

		m_pContext->ClearDepthStencilView(m_pCascadeShadowDSV[i], D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

		for (auto& iter : m_CascadeObjects[i])
		{
			if (FAILED(iter->Render_CascadeShadowDepth(i)))
				return E_FAIL;
			Safe_Release(iter);
		}
		m_CascadeObjects[i].clear();


		for (auto& List : m_InstanceCascadeObjects)
		{
			if (List.second.empty() == false)
			{
				if (FAILED(List.second.front()->Render_CascadeShadowDepth_Instance(i)))
					return E_FAIL;
			}
		}

		if (FAILED(m_pTarget_Manager->End_MRT(m_pContext)))
			return E_FAIL;

	}

	for (auto& List : m_InstanceCascadeObjects)
	{
		if (List.second.empty() == false)
		{
			for (auto& iter : List.second)
			{
				Safe_Release(iter);
			}
			List.second.clear();
		}
	}

	return S_OK;
}

HRESULT CRenderer::Render_Mouse()
{
	for (auto& iter : m_RenderObjects[RENDERGROUP::RENDER_MOUSE])
	{
		if (FAILED(iter->Render()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_RenderObjects[RENDER_MOUSE].clear();

	return S_OK;
}

HRESULT CRenderer::Render_Debug()
{
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pMRTShader->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_GameObjects"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;


	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_SSAO_Blur_V"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effects"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Decals"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Lights"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_SSAO_Blur_V"), m_pMRTShader, m_pVIBuffer)))
	
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Effects"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Decals"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;

	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_PrePostProcessScene"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_BloomDownSample1"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_BloomBlur_HV_5x5"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_SSR"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Cascade1"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Cascade2"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_Cascade3"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_OutlineObjects_Blend"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_DOF_Blur_HV"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;
	//if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_EffectShade"), m_pMRTShader, m_pVIBuffer)))
	//	return E_FAIL;

	/*if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_ShadowDepth"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;

	if (FAILED(m_pTarget_Manager->Render(TEXT("MRT_StaticShadowDepth"), m_pMRTShader, m_pVIBuffer)))
		return E_FAIL;*/

	return S_OK;
}

HRESULT CRenderer::Render_DebugObject()
{
	for (auto& iter : m_DebugRenderObjects)
	{
		if (FAILED(iter->Render_Debug()))
			return E_FAIL;
		Safe_Release(iter);
	}
	m_DebugRenderObjects.clear();

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

	return S_OK;
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

		TextureDesc.Width = (_uint)(m_fShadowTargetSizeRatio * ViewportDesc.Width);
		TextureDesc.Height = (_uint)(m_fShadowTargetSizeRatio * ViewportDesc.Height);
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

	////StaticShadowDepth
	//{
	//	ID3D11Texture2D* pDepthStencilTexture = nullptr;

	//	D3D11_TEXTURE2D_DESC	TextureDesc;
	//	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	//	TextureDesc.Width = (_uint)(m_fStaticShadowTargetSizeRatio * ViewportDesc.Width);
	//	TextureDesc.Height = (_uint)(m_fStaticShadowTargetSizeRatio * ViewportDesc.Height);
	//	TextureDesc.MipLevels = 1;
	//	TextureDesc.ArraySize = 1;
	//	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	//	TextureDesc.SampleDesc.Quality = 0;
	//	TextureDesc.SampleDesc.Count = 1;

	//	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	//	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	//	TextureDesc.CPUAccessFlags = 0;
	//	TextureDesc.MiscFlags = 0;

	//	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)))
	//		return E_FAIL;

	//	if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pStaticShadowDSV)))
	//		return E_FAIL;
	//}

	return S_OK;
}

HRESULT CRenderer::Ready_BlurData()
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

	return S_OK;
}

HRESULT CRenderer::Ready_DOF()
{
	m_pDOFShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_DepthOfField.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements);
	if (nullptr == m_pDOFShader)
		return E_FAIL;
	if (FAILED(m_pDOFShader->Initialize()))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Ready_Bloom()
{
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	ID3D11Texture2D* pDepthStencilTexture = nullptr;
	D3D11_TEXTURE2D_DESC	TextureDesc;

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / m_fSampleRatio5x5);
	TextureDesc.Height = (_uint)(0.25f * ViewportDesc.Height / m_fSampleRatio5x5);
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)) ||
		FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSample5x5_DSV)))
		return E_FAIL;

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / m_fSampleRatio25x25);
	TextureDesc.Height = (_uint)(ViewportDesc.Height / m_fSampleRatio25x25);
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)) ||
		FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSample25x25_DSV)))
		return E_FAIL;

	ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

	TextureDesc.Width = (_uint)(ViewportDesc.Width / m_fSampleRatio125x125);
	TextureDesc.Height = (_uint)(ViewportDesc.Height / m_fSampleRatio125x125);
	TextureDesc.MipLevels = 1;
	TextureDesc.ArraySize = 1;
	TextureDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	TextureDesc.SampleDesc.Quality = 0;
	TextureDesc.SampleDesc.Count = 1;
	TextureDesc.Usage = D3D11_USAGE_DEFAULT;
	TextureDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL/*| D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE*/;
	TextureDesc.CPUAccessFlags = 0;
	TextureDesc.MiscFlags = 0;

	if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &pDepthStencilTexture)) ||
		FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pDownSample125x125_DSV)))
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Execute_BloomBlur()
{


	return S_OK;
}

HRESULT CRenderer::Ready_SSAO()
{
	m_pSSAOShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_SSAO.hlsl"), VTXNORTEX::Elements, VTXNORTEX::iNumElements);
	if (nullptr == m_pSSAOShader)
		return E_FAIL;
	if (FAILED(m_pSSAOShader->Initialize()))
		return E_FAIL;
	
	random_device		RandomDevice;

	mt19937_64							RandomNumber(RandomDevice());
	uniform_real_distribution<_float>	Random(0.25f, 1.0f);

	// Start with 14 uniformly distributed vectors.  We choose the 8 corners of the cube
	// and the 6 center points along each cube face.  We always alternate the points on 
	// opposites sides of the cubes.  This way we still get the vectors spread out even
	// if we choose to use less than 14 samples.

	// 8 cube corners
	m_vOffsets[0] = XMFLOAT4(+1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[1] = XMFLOAT4(-1.0f, -1.0f, -1.0f, 0.0f);

	m_vOffsets[2] = XMFLOAT4(-1.0f, +1.0f, +1.0f, 0.0f);
	m_vOffsets[3] = XMFLOAT4(+1.0f, -1.0f, -1.0f, 0.0f);

	m_vOffsets[4] = XMFLOAT4(+1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[5] = XMFLOAT4(-1.0f, -1.0f, +1.0f, 0.0f);

	m_vOffsets[6] = XMFLOAT4(-1.0f, +1.0f, -1.0f, 0.0f);
	m_vOffsets[7] = XMFLOAT4(+1.0f, -1.0f, +1.0f, 0.0f);

	// 6 centers of cube faces
	m_vOffsets[8] = XMFLOAT4(-1.0f, 0.0f, 0.0f, 0.0f);
	m_vOffsets[9] = XMFLOAT4(+1.0f, 0.0f, 0.0f, 0.0f);

	m_vOffsets[10] = XMFLOAT4(0.0f, -1.0f, 0.0f, 0.0f);
	m_vOffsets[11] = XMFLOAT4(0.0f, +1.0f, 0.0f, 0.0f);

	m_vOffsets[12] = XMFLOAT4(0.0f, 0.0f, -1.0f, 0.0f);
	m_vOffsets[13] = XMFLOAT4(0.0f, 0.0f, +1.0f, 0.0f);

	for (int32 i = 0; i < 14; ++i)
	{
		// Create random lengths in [0.25, 1.0].
		_float fRandom = Random(RandomNumber);

		XMVECTOR v = fRandom * ::XMVector4Normalize(::XMLoadFloat4(&m_vOffsets[i]));

		::XMStoreFloat4(&m_vOffsets[i], v);
	}

	_float fFarZ = 1200.f;
	_float fAspect = 16.f / 9.f;
	_float fFovy = XMConvertToRadians(60.0f);

	_float halfHeight = fFarZ * tanf(0.5f * fFovy);
	_float halfWidth = fAspect * halfHeight;

	m_vFrustumFarCorner[0] = XMFLOAT4(-halfWidth, -halfHeight, fFarZ, 0.0f);
	m_vFrustumFarCorner[1] = XMFLOAT4(-halfWidth, +halfHeight, fFarZ, 0.0f);
	m_vFrustumFarCorner[2] = XMFLOAT4(+halfWidth, +halfHeight, fFarZ, 0.0f);
	m_vFrustumFarCorner[3] = XMFLOAT4(+halfWidth, -halfHeight, fFarZ, 0.0f);

	// RandomVectorTexture
	CTexture* pRandomTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Effects/RandomTexture.png"));

	m_pRandomSRV = pRandomTexture->Get_SRV();
	if(nullptr == m_pRandomSRV)
		return E_FAIL;

	Safe_AddRef(m_pRandomSRV);
	Safe_Release(pRandomTexture);

	return S_OK;
}

HRESULT CRenderer::Ready_IBL()
{
	m_pIrradianceTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/IrradianceMap/IrradianceMap%d.dds"), 23);
	m_pPreFilteredTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/PreFilteredMap/PreFilteredMap%d.dds"), 23);
	m_pBRDFTexture = CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/BackGround/Brdf/Brdf1.dds"));

	if (nullptr == m_pIrradianceTexture || nullptr == m_pPreFilteredTexture || nullptr == m_pBRDFTexture)
		return E_FAIL;

	return S_OK;
}

HRESULT CRenderer::Ready_SSR()
{
	m_tSSR_Data[0].fSSRStep = 0.0f;
	m_tSSR_Data[0].iSSRStepCount = 0;

	m_tSSR_Data[1].fSSRStep = 0.0125f;
	m_tSSR_Data[1].iSSRStepCount = 30;

	m_tSSR_Data[2].fSSRStep = 0.008333333f;
	m_tSSR_Data[2].iSSRStepCount = 45;

	m_tSSR_Data[3].fSSRStep = 0.00625f;
	m_tSSR_Data[3].iSSRStepCount = 60;

	m_tSSR_Data[4].fSSRStep = 0.005f;
	m_tSSR_Data[4].iSSRStepCount = 75;

	return S_OK;
}

void CRenderer::Set_ScreenShot(_bool bShoot, wstring strPath)
{
	if(TEXT("") == strPath)
		m_iScreenShotFrame = 0;

	m_bScreenShot = bShoot;  
	m_strScreenShotPath = strPath; 
}

HRESULT CRenderer::Ready_CascadeShadow()
{
	D3D11_VIEWPORT		ViewportDesc;

	_uint				iNumViewports = 1;
	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	//ShadowDepth
	for(_uint i = 0; i < 3; ++i)
	{
		ID3D11Texture2D* pDepthStencilTexture = nullptr;

		D3D11_TEXTURE2D_DESC	TextureDesc;
		ZeroMemory(&TextureDesc, sizeof(D3D11_TEXTURE2D_DESC));

		TextureDesc.Width = 1600.0f;
		TextureDesc.Height = 900.0f;
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

		if (FAILED(m_pDevice->CreateDepthStencilView(pDepthStencilTexture, nullptr, &m_pCascadeShadowDSV[i])))
			return E_FAIL;
	}

	return S_OK;
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

	Safe_Release(m_pSSAOShader);
	Safe_Release(m_pSSRShader);
	Safe_Release(m_pDOFShader);
	Safe_Release(m_pBloomShader);
	Safe_Release(m_pOutlineBlurShader);
	Safe_Release(m_pVIBuffer);
	Safe_Release(m_pBRDFTexture);
	Safe_Release(m_pVIBufferSSAO);

}
