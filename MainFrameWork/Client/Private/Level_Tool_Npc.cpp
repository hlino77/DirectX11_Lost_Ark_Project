#include "stdafx.h"
#include "Level_Tool_Npc.h"
#include "UI.h"
#include "EffectTool.h"
#include "Camera_Free.h"
#include "PartObject.h"
#include "StaticModel.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include "QuadTreeMgr.h"
#include <filesystem>
#include "NpcTool.h"

#include "GameInstance.h"

CLevel_Tool_Npc::CLevel_Tool_Npc(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool_Npc::Initialize()
{
	m_pGameInstance = GET_INSTANCE(CGameInstance);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\malgun.ttf", 16.0f, NULL, io.Fonts->GetGlyphRangesKorean());
	// Setup Dear ImGui style
	//ImGui::StyleColorsDark();
	ImGui::StyleColorsLight();

	// Setup Platform/Renderer backends
	//ImGui_ImplWin32_Init(g_hWnd);
	//ImGui_ImplDX11_Init(m_pDevice, m_pContext);

	if (FAILED(Ready_Light()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_Camera()))
		return E_FAIL;

	if (FAILED(Ready_Tools()))
		return E_FAIL;

	if (FAILED(Load_MapData(LEVEL_TOOL_NPC, TEXT("../Bin/Resources/MapData/BernCastle.data"))))
	{
		return E_FAIL;
	}

	m_pCamera->Get_TransformCom()->Set_State(CTransform::STATE_POSITION, Vec3(100.f, 20.f, 100.f));

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Tick(const _float& fTimeDelta)
{
	if (!m_IsImGUIReady)
	{
		m_IsImGUIReady = true;
	}

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	m_pNpcTool->Tick(fTimeDelta);
	m_pCamera->Tick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool_Npc::LateTick(const _float& fTimeDelta)
{
	m_pNpcTool->LateTick(fTimeDelta);
	m_pCamera->LateTick(fTimeDelta);

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Render_Debug()
{
	if (m_IsImGUIReady)
	{
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	}

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Tool_Npc::Ready_Light()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;

	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(0.705f, -0.667f, -0.239f, 0.f);
	LightDesc.vDirection.Normalize();
	LightDesc.vDiffuse = Vec4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = Vec4(1.0f, 1.0f, 1.0f, 1.f);
	LightDesc.vSpecular = Vec4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(pGameInstance->Add_Light(m_pDevice, m_pContext, LightDesc)))
		return E_FAIL;


	Vec3 vLook = LightDesc.vDirection;
	vLook.Normalize();

	Vec3 vOffset = Vec3(-117.04f, 110.27f, 36.20f);

	pGameInstance->Ready_StaticLightMatrix(vOffset, vLook);
	vOffset.Normalize();
	vOffset *= 30.0f;
	pGameInstance->Ready_LightMatrix(vOffset, vLook);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Ready_Camera()
{
	CCamera::CAMERADESC tCameraDesc;

	tCameraDesc.iLayer = (_uint)LAYER_TYPE::LAYER_CAMERA;
	tCameraDesc.vEye = Vec4(0.f, 3.f, -3.f, 1.f);
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

HRESULT CLevel_Tool_Npc::Ready_Tools()
{
	m_pNpcTool = CNpcTool::Create(m_pDevice, m_pContext, this);
	if (nullptr == m_pNpcTool)
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool_Npc::Load_MapData(LEVELID eLevel, const wstring& szFullPath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFullPath, FileMode::Read);

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

		pObject->Get_TransformCom()->Set_WorldMatrix(matWorld);


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

CLevel_Tool_Npc* CLevel_Tool_Npc::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_Tool_Npc* pInstance = new CLevel_Tool_Npc(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Tool_Npc");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Tool_Npc::Free()
{
	Super::Free();

	RELEASE_INSTANCE(CGameInstance);
	ImGui::DestroyContext();
}