#include "stdafx.h"
#include "Level_Lobby.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"
#include <filesystem>
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "StaticModel.h"
#include "UI.h"
#include "UI_Manager.h"
#include "Camera_Lobby.h"
#include <filesystem>
#include "QuadTreeMgr.h"
#include "Engine_Defines.h"
#include "Player_Select.h"
#include "UI_Tool.h"
#include "UI_Lobby.h"
#include "UI_Lobby_EntranceServer_Button.h"
#include "UI_Lobby_NickNameChange.h"
#include "Pool.h"
#include "Effect_Custom_EarthEaterParticle.h"
#include "Effect_Custom_EarthEaterSmallParticle.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_BACKGROUND)))
		return E_FAIL;

	if (FAILED(Ready_CameraFree(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Load_MapData(LEVEL_LOBBY, TEXT("../Bin/Resources/MapData/Character_Select_Lobby_ver2.data"))))
		return E_FAIL;

	if (FAILED(Ready_Mannequin()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Ready_EffectPool()))
		return E_FAIL;

	Start_QuadTree();

	m_bConnect = false;
	CGameInstance::GetInstance()->StopSound(CHANNEL_BGM);
	CGameInstance::GetInstance()->PlayBGM(L"Sunrise Horizon.wav", CHANNEL_BGM, g_fVolume);
	return S_OK;
}

HRESULT CLevel_Lobby::Tick(const _float& fTimeDelta)
{


	CUI* pUI = CUI_Manager::GetInstance()->Find_UIPart(LEVELID::LEVEL_LOBBY, TEXT("UI_Lobby"), TEXT("Button_Entrance_to_Server"));
	if (nullptr == pUI)
		return E_FAIL;

	Select_Player();

	if (m_eSelectClass == CHR_CLASS::CLASSEND)
		static_cast<CUILobby_Entrance_to_ServrerButton*>(pUI)->Set_Entrance(false);
	else if ((m_eSelectClass != CHR_CLASS::CLASSEND) && (static_cast<CUILobby_Entrance_to_ServrerButton*>(pUI)->Get_Entrance()))
	{
		if (m_bConnect == false)
		{
			m_bConnect = true;

			pUI = CUI_Manager::GetInstance()->Find_UIPart(LEVEL_LOBBY, TEXT("UI_Lobby"), TEXT("Lobby_NameChanger"));
			CServerSessionManager::GetInstance()->Set_Class((_uint)m_eSelectClass);
			CServerSessionManager::GetInstance()->Set_NickName(static_cast<CUI_Lobby_NickNameChange*>(pUI)->Get_NickName());//(L"HellowWorld");


			SetWindowText(g_hWnd, TEXT("서버에 접속중입니다."));

			CClientPacketHandler::Init();

			this_thread::sleep_for(1s);

			ClientServiceRef service = make_shared<ClientService>(
				NetAddress(SERVER_IP, SERVER_PORT),
				make_shared<IocpCore>(),
				make_shared<CServerSession>, // TODO : SessionManager 등
				1);

			ASSERT_CRASH(service->Start());

			for (int32 i = 0; i < 3; i++)
			{
				ThreadManager::GetInstance()->Launch([=]()
				{
					while (true)
					{
						service->GetIocpCore()->Dispatch();
					}
				});
			}
		}
		return S_OK;
	}

	
	return S_OK;
}

HRESULT CLevel_Lobby::LateTick(const _float& fTimeDelta)
{
	//CUI_Tool::GetInstance()->LateTick();
	return S_OK;
}

HRESULT CLevel_Lobby::Render_Debug()
{
	//CUI_Tool::GetInstance()->Tick();

	return S_OK;
}

HRESULT CLevel_Lobby::Exit()
{
	End_QuadTree();

	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (FAILED(pGameInstance->Clear_LightShadowTexture()))
		return E_FAIL;

	Safe_Release(pGameInstance);

	CGameInstance::GetInstance()->StopSoundAll();
	CUI_Manager::GetInstance()->Clear(LEVELID::LEVEL_LOBBY);
	CGameInstance::GetInstance()->Reset_Lights();
	CGameInstance::GetInstance()->Delete_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, m_pCamera);

	//CUI_Tool::GetInstance()->Set_ToolMode(false);


	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Lights()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	LIGHTDESC			LightDesc;
	ZeroMemory(&LightDesc, sizeof(LIGHTDESC));
	LightDesc.eType = LIGHTDESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = Vec4(0.239f, -0.667f, 0.705f, 0.f);
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

HRESULT CLevel_Lobby::Ready_EffectPool()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	

	for (_uint i = 0; i < 10; ++i)
	{
		CEffect_Custom_EarthEaterParticle* pEffect = dynamic_cast<CEffect_Custom_EarthEaterParticle*>(pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_EarthEaterParticle"));
		CPool<CEffect_Custom_EarthEaterParticle>::Return_Obj(pEffect);
	}

	for (_uint i = 0; i < 30; ++i)
	{
		CEffect_Custom_EarthEaterSmallParticle* pEffect = dynamic_cast<CEffect_Custom_EarthEaterSmallParticle*>(pGameInstance->Add_GameObject((_uint)LEVELID::LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_EFFECT, L"Prototype_GameObject_Effect_Custom_EarthEaterSmallParticle"));
		CPool<CEffect_Custom_EarthEaterSmallParticle>::Return_Obj(pEffect);
	}


	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_uint iLevelIndex = (_uint)pGameInstance->Get_CurrLevelIndex();

	CGameObject* pUI = pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_BackGround_Lobby"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI((LEVELID)iLevelIndex, static_cast<CUI*>(pUI));
;
	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_CameraFree(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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

	m_pCamera = pGameInstance->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, L"Prototype_GameObject_Camera_Lobby", &tCameraDesc);
	if(nullptr == m_pCamera)
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_Player(const LAYER_TYPE eLayerType)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Lobby::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	_uint iLevelIndex= (_uint)pGameInstance->Get_CurrLevelIndex();

	CGameObject* pUI = pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Lobby"));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI((LEVELID)iLevelIndex, static_cast<CUI*>(pUI));


	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Option")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		CUI_Manager::GetInstance()->Add_UI((LEVELID)LEVEL_STATIC, static_cast<CUI*>(pUI));

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Lobby::Load_MapData(LEVELID eLevel, const wstring& szFullPath)
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
	L"../Bin/Resources/Export/Lobby/"
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

		CGameObject* pObject = pGameInstance->Add_GameObject(eLevel, Desc.iLayer, TEXT("Prototype_GameObject_StaticModel_Lobby"), &Desc);

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

HRESULT CLevel_Lobby::Ready_Mannequin()
{
	CGameInstance* pGameInstance = GET_INSTANCE(CGameInstance);

	CGameObject* pObject = pGameInstance->Add_GameObject(LEVEL_LOBBY, (_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Prototype_GameObject_Player_Select_GN"));
	if (nullptr == pObject)
		return E_FAIL;

	m_pPC_Select[(_uint)CHR_CLASS::GUNSLINGER] = pObject;

	pObject = pGameInstance->Add_GameObject(LEVEL_LOBBY, (_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Prototype_GameObject_Player_Select_WR"));
	if (nullptr == pObject)
		return E_FAIL;

	m_pPC_Select[(_uint)CHR_CLASS::SLAYER] = pObject;

	pObject = pGameInstance->Add_GameObject(LEVEL_LOBBY, (_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Prototype_GameObject_Player_Select_WDR"));
	if (nullptr == pObject)
		return E_FAIL;

	m_pPC_Select[(_uint)CHR_CLASS::DESTROYER] = pObject;

	pObject = pGameInstance->Add_GameObject(LEVEL_LOBBY, (_uint)LAYER_TYPE::LAYER_PLAYER, TEXT("Prototype_GameObject_Player_Select_SP"));
	if (nullptr == pObject)
		return E_FAIL;

	m_pPC_Select[(_uint)CHR_CLASS::DOAGA] = pObject;

	RELEASE_INSTANCE(CGameInstance);

	return S_OK;
}

void CLevel_Lobby::Select_Player()
{
	CUI_Manager* pUIManager = CUI_Manager::GetInstance();
	Safe_AddRef(pUIManager);
	_uint iLevelIndex =	CGameInstance::GetInstance()->Get_CurrLevelIndex();

	if (true == static_cast<CPlayer_Select*>(m_pPC_Select[(_uint)CHR_CLASS::GUNSLINGER])->Is_Selected())
	{
		m_eSelectClass = CHR_CLASS::GUNSLINGER;
	}
	else if (true == static_cast<CPlayer_Select*>(m_pPC_Select[(_uint)CHR_CLASS::SLAYER])->Is_Selected())
	{
		m_eSelectClass = CHR_CLASS::SLAYER;
	}
	else if (true == static_cast<CPlayer_Select*>(m_pPC_Select[(_uint)CHR_CLASS::DESTROYER])->Is_Selected())
	{
		m_eSelectClass = CHR_CLASS::DESTROYER;
	}
	else if (true == static_cast<CPlayer_Select*>(m_pPC_Select[(_uint)CHR_CLASS::DOAGA])->Is_Selected())
	{
		m_eSelectClass = CHR_CLASS::DOAGA;
	}
	else
	{
		m_eSelectClass = CHR_CLASS::CLASSEND;
	}

	Safe_Release(pUIManager);
}

void CLevel_Lobby::Start_QuadTree()
{
	m_pQuadTreeThread = new thread([=]()
		{
			CGameInstance* pGameInstance = CGameInstance::GetInstance();
			Safe_AddRef(pGameInstance);

			CQuadTreeMgr* pQuadTreeManager = CQuadTreeMgr::GetInstance();
			pQuadTreeManager->AddRef();

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_QuadTree"))))
				return FALSE;

			if (FAILED(pGameInstance->Add_Timer(TEXT("Timer_QuadTree_60"))))
				return FALSE;

			_float		fTimeAcc = 0.f;


			while (!pQuadTreeManager->Is_End())
			{
				fTimeAcc += pGameInstance->Compute_TimeDelta(TEXT("Timer_QuadTree"));

				if (fTimeAcc >= 1.f / 60.0f)
				{
					pQuadTreeManager->Tick(pGameInstance->Compute_TimeDelta(TEXT("Timer_QuadTree_60")));
					fTimeAcc = 0.f;
				}
			}

			Safe_Release(pQuadTreeManager);
			Safe_Release(pGameInstance);

			CGameInstance::GetInstance()->Delete_Timer(L"Timer_QuadTree");
			CGameInstance::GetInstance()->Delete_Timer(L"Timer_QuadTree_60");

			return TRUE;
		});
}

void CLevel_Lobby::End_QuadTree()
{
	if (m_pQuadTreeThread == nullptr)
		return;

	CQuadTreeMgr::GetInstance()->Set_End(true);
	m_pQuadTreeThread->join();
	CQuadTreeMgr::GetInstance()->Reset_QaudTree();
	Safe_Delete(m_pQuadTreeThread);
}


CLevel_Lobby * CLevel_Lobby::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Lobby*	pInstance = new CLevel_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Lobby::Free()
{
 	__super::Free();
}
