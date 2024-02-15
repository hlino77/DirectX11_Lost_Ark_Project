#include "stdafx.h"
#include "..\Public\MainApp.h"
#include <filesystem>
#include "tinyxml2.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "StateMachine.h"
#include "ServerSessionManager.h"
#include "ColliderSphere.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"
#include "SocketUtils.h"
#include "RigidBody.h"
#include "MainLogo.h"
#include "BackGround_Loading.h"
#include "ColliderOBB.h"
#include "ColliderDoughnut.h"
#include "ColliderSphereGroup.h"
#include "CollisionManager.h"
#include "ColliderFrustum.h"
#include "NavigationMgr.h"
#include "EventMgr.h"
#include "PhysXMgr.h"

#include "UseLock_Transform.h"
#include "Camera_Free.h"
#include "Player.h"
#include "Camera_Player.h"
#include "BackGround_MainLogo.h"
#include "Chat_Manager.h"
#include "UI_LoadingWnd.h"
#include "UI_LoadingLabelTop.h"
#include "UI_LoadingLabelBottom.h"
#include "UI_LoadingEmptyBar.h"
#include "UI_LoadingFill.h"
#include "UI_LoadingArrow.h"
#include "UI_LoadingShine.h"
#include "UI_Manager.h"
#include "UI_Mouse_Cursor.h"
#include "UI_Mouse_PickedIcon.h"
#include "UI_Mouse_EntranceParty.h"
#include "UI_OptionWnd.h"
#include "UI_Option_Sound.h"
#include "UI_Option_Video.h"
#include "UI_Option.h"
#include "BehaviorTree.h"
#include "UI_Tool.h"
#include "TextBox.h"
#include "LockFree_Transform.h"
#include "Projectile.h"
#include "UI_DamageFont.h"
#include "Pool.h"
#include "Damage_Manager.h"
#include "Effect_Manager.h"
#include "ThreadManager.h"
#include "UI_Loading.h"
#include "Effect.h"
#include "UI_SpeechBubble.h"
#include "QuadTreeMgr.h"
#include "Item_Manager.h"
#include "ClientEvent_BernStart.h"
#include "Sound_Manager.h"

namespace fs = std::filesystem;

_float g_fVolume;
_float g_fTimeDeltaRatio;

CMainApp::CMainApp()	
	: m_pGameInstance(CGameInstance::GetInstance())
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	g_fVolume = 0.5f;
	g_fTimeDeltaRatio = 1.0f;

	/* 1. 내 게임의 초기화를 수행할꺼야. */
	/* 1-1. 그래픽장치를 초기화한다. */
	/* 1-2. 사운드장치를 초기화한다. */
	/* 1-3. 입력장치를 초기화한다. */
	GRAPHIC_DESC		GraphicDesc;
	ZeroMemory(&GraphicDesc, sizeof GraphicDesc);

	GraphicDesc.eWinMode = GRAPHIC_DESC::WINMODE_WIN;
	GraphicDesc.hWnd = g_hWnd;
	GraphicDesc.iWinSizeX = g_iWinSizeX;
	GraphicDesc.iWinSizeY = g_iWinSizeY;

	SocketUtils::Init();

	if (FAILED(m_pGameInstance->Initialize_Engine(LEVEL_END, _uint(LAYER_TYPE::LAYER_END), GraphicDesc, &m_pDevice, &m_pContext, g_hWnd, g_hInstance)))
		return E_FAIL;
		
	if (FAILED(Initialize_Client()))
		return E_FAIL;

	if (FAILED(Ready_SoundManager()))
		return E_FAIL;

	if (FAILED(Ready_Event()))
		return E_FAIL;

	if (FAILED(Ready_Prototype_Component()))
		return E_FAIL;

	/* 1-4. 게임내에서 사용할 레벨(씬)을 생성한다.   */
	if (FAILED(Open_Level(LEVEL_LOGO)))
		return E_FAIL;

	/* 1-4-1. 게임내에서 사용할 여러 자원(텍스쳐, 모델, 객체) 들을 준비한다.  */

	if (FAILED(Ready_Prototype_Font()))
		return E_FAIL;
	
	if (FAILED(Ready_Prototype_Effect()))
		return E_FAIL;

	if (FAILED(Ready_Camera_Free()))
		return E_FAIL;
	

	if (FAILED(Loading_OptionUI()))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Tick(_float fTimeDelta)
{
	if (KEY_TAP(KEY::F1))
	{
		Active_Camera_Free();
	}
	if (KEY_TAP(KEY::F3))
	{
		CQuadTreeMgr::GetInstance()->Set_Stop(false);
	}
	if (KEY_TAP(KEY::F4))
	{
		CQuadTreeMgr::GetInstance()->Set_Stop(true);
	}

	if (KEY_TAP(KEY::F5))
	{
		g_fTimeDeltaRatio = 0.1f;
	}
	if (KEY_TAP(KEY::F6))
	{
		g_fTimeDeltaRatio = 1.0f;
	}

	if (KEY_HOLD(KEY::N) && KEY_TAP(KEY::NUM_1))
		CNavigationMgr::GetInstance()->OnOff_Render();

	//if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::S))
	//	m_pRenderer_Com->Set_StaticShadow();

	fTimeDelta *= g_fTimeDeltaRatio;

	m_pGameInstance->FinalTick(fTimeDelta);

	CEffect_Manager::GetInstance()->Tick(fTimeDelta);
	CServerSessionManager::GetInstance()->Tick(fTimeDelta);

	/* 게임내에 존재하는 여러 객체들의 갱신. */
	/* 레벨의 갱신 */
	m_pGameInstance->Tick(fTimeDelta);

	CChat_Manager::GetInstance()->CursurTick(fTimeDelta);
}

HRESULT CMainApp::Render()
{

	m_pGameInstance->Clear_BackBuffer_View(Vec4(0.f, 0.f, 0.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();

	
	if (FAILED(m_pRenderer_Com->Ready_InstanceRender()))
		return E_FAIL;

	/* 게임내에 존재하는 여러 객체들의 렌더링. */
	if (FAILED(m_pRenderer_Com->Draw()))
		return E_FAIL;

	if (FAILED(CChat_Manager::GetInstance()->Render()))
		return E_FAIL;

	//CNavigationMgr::GetInstance()->Render(m_pGameInstance->Get_CurrLevelIndex());

	if (FAILED(m_pGameInstance->Render_Debug()))
		return E_FAIL;

	/* 초기화한 장면에 객체들을 그린다. */
	if (FAILED(m_pGameInstance->Present()))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Open_Level(LEVELID eLevelID)
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* 로고레베릉ㄹ 할당하고 싶었지만. 로고레벨을 위한 로딩레벨을 먼저 생성하여 로딩작업을 수행할꺼야. */
	/* 로딩객체에게 eLevelID라는 내가 실제 할당ㅎ아고 싶었던 레벨열거체를 준거지?! 실제할당하고싶었던 레벨에 자원을 준비라하라고 */
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, eLevelID, L"None"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Initialize_Client()
{
	if (FAILED(CCollisionManager::GetInstance()->Reserve_Manager((_uint)LAYER_COLLIDER::LAYER_END)))
		return E_FAIL;
	if (FAILED(CNavigationMgr::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(CEventMgr::GetInstance()->Reserve_EventMgr(m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(CPhysXMgr::GetInstance()->ReserveManager()))
		return E_FAIL;
	if (FAILED(CChat_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext)))
		return E_FAIL;
	if (FAILED(CUI_Manager::GetInstance()->Reserve_Manager()))
		return E_FAIL;
	if (FAILED(CEffect_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;
	//CUI_Tool::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);
	if (FAILED(CItem_Manager::GetInstance()->Reserve_Manager(m_pDevice, m_pContext)))
		return E_FAIL;

	ThreadManager::GetInstance()->ReserveManager(0);

	if (FAILED(CUI_Tool::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext)))
		return E_FAIL;

	// Manager Reserve
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Component()
{
	if (nullptr == m_pGameInstance)
		return E_FAIL;

	/* SkyDome */
	{
		wstring strFilePath = L"../Bin/Resources/SkyDome/";

		Matrix matPivot = Matrix::Identity;
		XMStoreFloat4x4(&matPivot, XMMatrixScaling(1000.f, 1000.f, 1000.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)));

		for (_int i = 0; i < 12; ++i)
		{
			wstring strFileName = L"SkyDome" + ::to_wstring(i);
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

			if (SUCCEEDED(m_pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
			{
				if(4 == i)
					XMStoreFloat4x4(&matPivot, XMMatrixScaling(500.f, 500.f, 500.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)));

				if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
					CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, matPivot))))
					return E_FAIL;
			}
		}

		XMStoreFloat4x4(&matPivot, XMMatrixScaling(100.f, 100.f, 100.f) * XMMatrixRotationX(XMConvertToRadians(90.0f)));
		wstring strComponentName = L"Prototype_Component_Model_SkyFloor";

		if (SUCCEEDED(m_pGameInstance->Check_Prototype(LEVEL_STATIC, strComponentName)))
		{
			if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFilePath, L"SkyFloor", true, false, matPivot))))
				return E_FAIL;
		}
	}

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_SkyFloorNoise"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/SkyDome/SkyFloor/fx_d_noise_004.png"))))
		return E_FAIL;

	/* For.Prototype_Component_Renderer */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Renderer"), m_pRenderer_Com = CRenderer::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	Safe_AddRef(m_pRenderer_Com);

	/* For.Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Rect"),
		CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Cube */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Cube"),
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_VIBuffer_Point */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Point"),
		CVIBuffer_Point::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Particle */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Particle"),
		CVIBuffer_Particle::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	
	/* For.Prototype_Component_VIBuffer_Trail */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_VIBuffer_Trail"),
		CVIBuffer_Trail::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Transform */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_LockFree_Transform"),
		Engine::CLockFree_Transform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_UseLock_Transform"),
		CUseLock_Transform::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_RigidBody */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_RigidBody"),
		CRigidBody::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
	//	CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXCUBETEX_DECLARATION::Elements, VTXCUBETEX_DECLARATION::iNumElements))))
	//	return E_FAIL;

	/* For.Prototype_Component_Shader_VtxTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_VtxTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxTex.hlsl"), VTXTEX::Elements, VTXTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_State */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_StateMachine"),
		CStateMachine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Behavior */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_BehaviorTree"),
		CBehaviorTree::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereColider"),
		CSphereCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_SphereColliderGroup"),
		CSphereColliderGroup::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_OBBColider"),
		COBBCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_FrustumCollider"),
		CFrustumCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_DoughnutCollider"),
		CDoughnutCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LostArk_MainLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/MainLogo/Login_Logo%d.png", 36))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_MainLogo_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/MainLogo/BackGround.png"))))
		return E_FAIL;

	//신규로딩텍스처
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingBackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i1b.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_LoadingImgs"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/LoadingImg%d.png",5))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Frame"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/Loading_Rect.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Top"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i18_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Bottom"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i16_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_EmptyBar"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i7_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_FillBar"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i6_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_BarArrow"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/loading_i5_nopack.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Shine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Loading/Loading_Shine.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Basic_Mouse"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/Mouse/Basic_Mouse_ReSize.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Rank"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Item_Icon/Rank/Rank%d.png", 7))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EntranceNameWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Party_HPUI/EntranceNameWnd.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_EntranceWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Party_HPUI/EntranceWnd%d.png",2))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading"),
		CBackGround_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LostArk_MainLogo"),
		CBackGround_MainLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingWnd"),
		CUI_LoadingWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingLabel_Top"),
		CUI_LoadingLabelTop::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_LoadingLabel_Bottom"),
		CUI_LoadingLabelBottom::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_EmptyBar"),
		CUI_LoadingEmptyBar::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Fill"),
		CUI_LoadingFill::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Arrow"),
		CUI_LoadingArrow::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Loading_Shine"),
		CUI_LoadingShine::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Mouse_PickedIcon"),
		CUI_Mouse_PickedIcon::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MouseCursor"),
		CUI_Mouse_Cursor::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_MouseEntranceParty"),
		CUI_Mouse_EntranceParty::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_Model */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_Model"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ModelEffect"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_Effect.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_SkyDome"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxSkyDome.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_AnimModel */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModel"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel.hlsl"), VTXANIMMODEL::Elements, VTXANIMMODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_AnimModelInstance"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimModel_Instance.hlsl"), VTXINSTANCE_ANIMMODEL::Elements, VTXINSTANCE_ANIMMODEL::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectMesh"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectMesh.hlsl"), VTXMODEL::Elements, VTXMODEL::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTex"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectTex.hlsl"), VTXPOINT::Elements, VTXPOINT::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_ParticleSystem"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_ParticleSystem.hlsl"), VTXPARTICLE::Elements, VTXPARTICLE::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectDecal"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectDecal.hlsl"), VTXCUBETEX::Elements, VTXCUBETEX::iNumElements))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_EffectTrail"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_EffectTrail.hlsl"), VTXTRAIL::Elements, VTXTRAIL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Shader_StaticModelInstace"),
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxModel_Instance.hlsl"), VTXINSTANCE_MODEL::Elements, VTXINSTANCE_MODEL::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Free"),
		CCamera_Free::Create(m_pDevice, m_pContext, L"Free_Camera"))))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_TextBox"),
		CTextBox::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	CDamage_Manager::GetInstance()->Reserve_Manager(g_hWnd, m_pDevice, m_pContext);
	
	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Projectile"),
		CProjectile::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 200; ++i)
	{
		CProjectile* pProjectile = dynamic_cast<CProjectile*>(m_pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_SKILL, L"Prototype_GameObject_Projectile"));
		CPool<CProjectile>::Return_Obj(pProjectile);
	}


	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Middle"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Middle.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Up"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Up.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Chat_Bubble_Down"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Chat/Chat_Bubble_Down.png"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_SpeechBubble"),
		CUI_SpeechBubble::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	for (_uint i = 0; i < 30; ++i)
	{
		_uint iIndex = i;
		CUI_SpeechBubble* pUI = dynamic_cast<CUI_SpeechBubble*>(m_pGameInstance->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_SpeechBubble"), &iIndex));
		CPool<CUI_SpeechBubble>::Return_Obj(pUI);
	}

	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Font()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();

	pGameInstance->AddFont(L"125", L"../Bin/Resources/Fonts/125.spritefont");
	pGameInstance->AddFont(L"AdobeFont", L"../Bin/Resources/Fonts/AdobeFont.spritefont");
	pGameInstance->AddFont(L"Cascadia", L"../Bin/Resources/Fonts/Cascadia.spritefont");
	pGameInstance->AddFont(L"한컴고딕", L"../Bin/Resources/Fonts/한컴고딕.spritefont");
	pGameInstance->AddFont(L"단조", L"../Bin/Resources/Fonts/단조.spritefont");
	pGameInstance->AddFont(L"던파연마된칼날", L"../Bin/Resources/Fonts/던파연마된칼날.spritefont");
	pGameInstance->AddFont(L"넥슨Lv1고딕", L"../Bin/Resources/Fonts/넥슨Lv1고딕.spritefont");
	pGameInstance->AddFont(L"빛의계승자", L"../Bin/Resources/Fonts/빛의계승자.spritefont");
	pGameInstance->AddFont(L"넥슨Lv1고딕Bold", L"../Bin/Resources/Fonts/넥슨Lv1고딕Bold.spritefont");

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMainApp::Ready_Prototype_Effect()
{	
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	// Mesh
	wstring strMeshEffectFilePath = TEXT("../Bin/Resources/Effects/FX_Meshes/");
	for (const auto& category : fs::directory_iterator(strMeshEffectFilePath))
	{
		if (!category.is_directory())
			continue;
		for (const auto& entry : fs::directory_iterator(category.path()))
		{
			Matrix matPivot = Matrix::Identity;
			const wstring& strFileName = entry.path().stem();
			wstring strComponentName = L"Prototype_Component_Model_" + strFileName;
			wstring strFinalPath = category.path().generic_wstring() + TEXT("/");

			if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
				CModel::Create(m_pDevice, m_pContext, strFinalPath, strFileName, true, false))))
				return E_FAIL;
		}
	}

	// Effect
	

	CEffect_Manager::GetInstance()->Ready_EffectPool(10);

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

HRESULT CMainApp::Ready_Camera_Free()
{
	CCamera::CAMERADESC tCameraDesc;

	tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	tCameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	tCameraDesc.fNear = 0.2f;
	tCameraDesc.fFar = 1200.0f;
	tCameraDesc.TransformDesc.fRotationPerSec = 10.0f;
	tCameraDesc.TransformDesc.fSpeedPerSec = 10.0f;

	CGameObject* pCamera = CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free", &tCameraDesc);

	m_pCamera = dynamic_cast<CCamera_Free*>(pCamera);

	return S_OK;
}

HRESULT CMainApp::Loading_OptionUI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_OptionWnd"),
		CUI_OptionWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_OptionSound"),
		CUI_Option_Sound::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_OptionVideo"),
		CUI_Option_Video::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_UI_Option"),
		CUI_Option::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Button"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/Button%d.png", 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_CheckBox"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/Option_CheckBox%d.png", 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DetailButton"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/OptionDetailButton%d.png", 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Quit_Option"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/QuitOption%d.png", 2))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_OptionTag"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/OptionTag%d.png", 3))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragBar"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/DragBar.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_Line"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/Line.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_DragLine"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/Option_DragLine.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_OptionWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/OptionWnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Option_ValueTextWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Option/ValueTextWnd.png"))))
		return E_FAIL;

	CUI* pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Option")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI((LEVELID)LEVEL_STATIC, static_cast<CUI*>(pUI));

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CMainApp::Ready_SoundManager()
{
	CSound_Manager::GetInstance()->Add_ChannelGroup("BGM", 0.5f);
	CSound_Manager::GetInstance()->Add_ChannelGroup("UI", 0.5f);
	CSound_Manager::GetInstance()->Add_ChannelGroup("Ambient", 0.5f);
	CSound_Manager::GetInstance()->Add_ChannelGroup("Effect", 0.5f);

	return S_OK;
}

HRESULT CMainApp::Ready_Event()
{
	CEventMgr::GetInstance()->Add_Event(new CClientEvent_BernStart((_uint)EVENT::BERNSTART, m_pDevice, m_pContext));

	return S_OK;
}

void CMainApp::Active_Camera_Free()
{
	if (m_pCamera->Is_Active())
	{
		if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		{
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Active(true);
		}
		m_pCamera->Set_Active(false);
	}                                 
	else
	{
		if (nullptr != CServerSessionManager::GetInstance()->Get_Player())
		{
			CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Set_Active(false);
			m_pCamera->Get_TransformCom()->Set_WorldMatrix(CServerSessionManager::GetInstance()->Get_Player()->Get_Camera()->Get_TransformCom()->Get_WorldMatrix());
		}
		m_pCamera->Set_Active(true);
	}
}

CMainApp * CMainApp::Create()
{
	CMainApp*	pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Client::CMainApp::Free()
{	
	__super::Free();

	Safe_Release(m_pRenderer_Com);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
	CGameInstance::Release_Engine();

	SocketUtils::Clear();
}
