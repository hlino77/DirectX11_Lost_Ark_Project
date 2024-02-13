#include "stdafx.h"
#include "Level_Tool.h"
#include "UI.h"
#include "EffectTool.h"
#include "Effect_PcModel.h"
#include "Camera_Free.h"
#include "PartObject.h"
#include "QuadTreeMgr.h"
#include <filesystem>
#include "AsUtils.h"
#include "StaticModel.h"
#include "SkyFloor.h"
#include "GameInstance.h"

CLevel_Tool::CLevel_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	//ImGui_ImplWin32_Init(g_hWnd);
	//ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_SkyBox()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Camera()))
		return E_FAIL;
	
	if (FAILED(Ready_Tools()))
		return E_FAIL;

	if (FAILED(Load_MapData(LEVEL_TOOL, TEXT("../Bin/Resources/MapData/Chaos1.data"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Tick(const _float& fTimeDelta)
{
	if (!m_IsImGUIReady)
	{
		m_IsImGUIReady = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	
	if (m_pEffectTool)
		m_pEffectTool->Tick(fTimeDelta);

	if (m_pEffectModel)
		m_pEffectModel->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool::LateTick(const _float& fTimeDelta)
{
	if (m_pEffectTool)
		m_pEffectTool->LateTick(fTimeDelta);

	if (m_pEffectModel)
		m_pEffectModel->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool::Exit()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Clear_LightShadowTexture()))
		return E_FAIL;

	Safe_Release(pGameInstance);

	CGameInstance::GetInstance()->StopSoundAll();

	return S_OK;
}

HRESULT CLevel_Tool::Render_Debug()
{
	if (m_IsImGUIReady)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

		m_pEffectTool->DebugRender();
	}

	return S_OK;
}

HRESULT CLevel_Tool::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

CGameObject* CLevel_Tool::GetPivotObject()
{
	return m_pPivotObject;
}

void CLevel_Tool::SetPivotObject(CGameObject* pPartObject)
{
	m_pPivotObject = pPartObject;
}

HRESULT CLevel_Tool::Ready_Layer_SkyBox()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CGameObject* pSkyDome = pGameInstance->Add_GameObject(LEVEL_TOOL, _uint(LAYER_TYPE::LAYER_SKYBOX), TEXT("Prototype_GameObject_SkyDome"));
	if (nullptr == pSkyDome)
		return E_FAIL;

	CSkyFloor::SkyFloorDescription desc;

	// 첫 번째
	desc.vTileCount = Vec2(2.f, 2.f);
	desc.vFloorUVoffset = Vec2(0.005f, 0.005f);
	desc.fAlpha = 0.4f;
	CGameObject* pSkyFloor = pGameInstance->Add_GameObject(LEVEL_TOOL, _uint(LAYER_TYPE::LAYER_SKYBOX), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -138.f, 100.f));

	// 두 번째
	desc.vTileCount = Vec2(4.f, 4.f);
	desc.vFloorUVoffset = Vec2(0.0047f, 0.0047f);
	desc.fAlpha = 0.4f;
	pSkyFloor = pGameInstance->Add_GameObject(LEVEL_TOOL, _uint(LAYER_TYPE::LAYER_SKYBOX), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -136.f, 100.f));

	// 세 번째
	desc.vTileCount = Vec2(5.f, 5.f);
	desc.vFloorUVoffset = Vec2(0.005f, 0.005f);
	desc.fAlpha = 0.4f;
	pSkyFloor = pGameInstance->Add_GameObject(LEVEL_TOOL, _uint(LAYER_TYPE::LAYER_SKYBOX), TEXT("Prototype_GameObject_SkyFloor"), &desc);
	if (nullptr == pSkyFloor)
		return E_FAIL;

	pSkyFloor->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, -134.f, 100.f));

	CRenderer::Set_IBLTexture(22);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(-0.353f, -0.734f, -0.579f, 0.f);
	LightDesc.vDirection.Normalize();
	LightDesc.vDiffuse = Vec4(0.3f, 0.3f, 0.3f, 1.f);
	LightDesc.vAmbient = Vec4(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);


	CTexture* pStaticShadowMap = CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/LightMap/Light_Chaos1.dds");

	if (FAILED(pGameInstance->Set_LightShadowTexture(pStaticShadowMap)))
		return E_FAIL;
	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;

	Vec3 vLook = LightDesc.vDirection;
	vLook.Normalize();
	//Vec3 vPos = Vec3(-80.78f, 83.75f, 33.80f);
	//Matrix matLightView = Matrix::CreateWorld(vPos, -vLook, Vec3(0.0f, 1.0f, 0.0f));

	Vec3 vOffset = Vec3(129.90f, 19.58f, 121.53f);

	pGameInstance->Ready_StaticLightMatrix(vOffset, vLook);
	vOffset = vLook * -30.0f;
	pGameInstance->Ready_LightMatrix(vOffset, vLook);


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Camera()
{
	//CCamera::CAMERADESC tCameraDesc;

	//tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	//tCameraDesc.vEye = Vec4(0.f, 3.f, -3.f, 1.f);
	//tCameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	//tCameraDesc.fFovy = XMConvertToRadians(60.0f);
	//tCameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	//tCameraDesc.fNear = 0.2f;
	//tCameraDesc.fFar = 1200.0f;
	//tCameraDesc.TransformDesc.fRotationPerSec = 10.0f;
	//tCameraDesc.TransformDesc.fSpeedPerSec = 10.0f;

	//CGameObject* pCamera = CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free", &tCameraDesc);

	//m_pCamera = dynamic_cast<CCamera_Free*>(pCamera);

	m_pCamera = dynamic_cast<CCamera_Free*>(CGameInstance::GetInstance()->Find_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Free"));
	m_pCamera->Set_Active(true);

	return S_OK;
}

HRESULT CLevel_Tool::Ready_Tools()
{
	m_pEffectTool = CEffectTool::Create(m_pDevice, m_pContext, this);
	//m_pMediator->SetPrefabsView(m_pPrefabsView);

	m_pEffectModel = CEffect_PcModel::Create(m_pDevice, m_pContext, this);
	return S_OK;
}

HRESULT CLevel_Tool::Load_MapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

	//Matrix		PivotMatrix = XMMatrixIdentity();
	//PivotMatrix = XMMatrixRotationX(XMConvertToRadians(90.0f));


	Vec3	QuadTreePosition = {};
	Vec3	QuadTreeScale = {};
	_uint	QuadTreeMaxDepth = {};

	file->Read<Vec3>(QuadTreePosition);
	file->Read<Vec3>(QuadTreeScale);
	file->Read<_uint>(QuadTreeMaxDepth);

	CQuadTreeMgr::GetInstance()->Make_QaudTree(QuadTreePosition, QuadTreeScale, QuadTreeMaxDepth);


	vector<wstring> paths =
	{
	L"../Bin/Resources/Export/Bern/",
	L"../Bin/Resources/Export/Chaos1/",
	L"../Bin/Resources/Export/Chaos2/",
	L"../Bin/Resources/Export/Chaos3/",
	L"../Bin/Resources/Export/Boss/"
	};


	_uint iSize = file->Read<_uint>();
	bool fileFound = false;

	for (_uint i = 0; i < iSize; ++i)
	{

		string strFileName = file->Read<string>();
		wstring selectedPath = {};

		for (const auto& path : paths)
		{
			wstring fullPath = path + CAsUtils::ToWString(strFileName);

			if (std::filesystem::exists(fullPath))
			{
				selectedPath = path;
			}
		}

		if (selectedPath.empty())
		{
			MessageBox(g_hWnd, L"File not found in any specified paths.", L"Error", MB_OK);
			return E_FAIL;
		}

		Matrix	matWorld = file->Read<Matrix>();
		_bool bInstance = false;
		file->Read<_bool>(bInstance);

		CStaticModel::MODELDESC Desc;
		Desc.strFileName = CAsUtils::ToWString(strFileName);
		Desc.strFilePath = selectedPath;
		Desc.iLayer = (_uint)LAYER_TYPE::LAYER_BACKGROUND;
		Desc.IsMapObject = true;
		Desc.bInstance = bInstance;

		CGameObject* pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel"), &Desc);

		if (nullptr == pObject)
		{
			Safe_Release(pGameInstance);
			return E_FAIL;
		}

		matWorld._41 -= 117.f;
		matWorld._42 -= 0.2f;
		matWorld._43 -= 98.f;
		pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);
		//m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_STATICSHADOW, pObject);

		_uint			QuadTreeSize = {};

		file->Read<_uint>(QuadTreeSize);

		for (size_t i = 0; i < QuadTreeSize; i++)
		{
			_uint Index = {};
			file->Read<_uint>(Index);

			pObject->Add_QuadTreeIndex(Index);
			CQuadTreeMgr::GetInstance()->Add_Object(pObject, Index);
		}

	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Tool * CLevel_Tool::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Tool*	pInstance = new CLevel_Tool(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool::Free()
{
	Safe_Release(m_pEffectTool);
	Safe_Release(m_pEffectModel);

	Super::Free();

	RELEASE_INSTANCE(CGameInstance);
	ImGui::DestroyContext();
}