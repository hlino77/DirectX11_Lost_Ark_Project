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

CLevel_Lobby::CLevel_Lobby(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(LAYER_TYPE::LAYER_UI)))
		return E_FAIL;

	if (FAILED(Ready_CameraFree(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

	if (FAILED(Load_MapData(LEVEL_LOBBY, TEXT("../Bin/Resources/MapData/Character_Select_Lobby_ver2.data"))))
		return E_FAIL;

	Start_QuadTree();

	m_bConnect = false;

	return S_OK;
}

HRESULT CLevel_Lobby::Tick(const _float& fTimeDelta)
{

	if (KEY_HOLD(KEY::ENTER) && KEY_TAP(KEY::NUM_1))
	{
		if (m_bConnect == false)
		{
			m_bConnect = true;

			CServerSessionManager::GetInstance()->Set_Class((_uint)CHR_CLASS::SLAYER);
			CServerSessionManager::GetInstance()->Set_NickName(L"HellowWorld");


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

	if (KEY_HOLD(KEY::ENTER) && KEY_TAP(KEY::NUM_2))
	{
		if (m_bConnect == false)
		{
			m_bConnect = true;

			CServerSessionManager::GetInstance()->Set_Class((_uint)CHR_CLASS::GUNSLINGER);
			CServerSessionManager::GetInstance()->Set_NickName(L"HellowWorld");


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

	if (KEY_HOLD(KEY::ENTER) && KEY_TAP(KEY::NUM_3))
	{
		if (m_bConnect == false)
		{
			m_bConnect = true;

			CServerSessionManager::GetInstance()->Set_Class((_uint)CHR_CLASS::DESTROYER);
			CServerSessionManager::GetInstance()->Set_NickName(L"HellowWorld");


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

	if (KEY_HOLD(KEY::ENTER) && KEY_TAP(KEY::NUM_4))
	{
		if (m_bConnect == false)
		{
			m_bConnect = true;

			CServerSessionManager::GetInstance()->Set_Class((_uint)CHR_CLASS::BARD);
			CServerSessionManager::GetInstance()->Set_NickName(L"HellowWorld");


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
	return S_OK;
}

HRESULT CLevel_Lobby::Exit()
{
	End_QuadTree();

	CGameInstance::GetInstance()->StopSoundAll();
	CUI_Manager::GetInstance()->Clear(LEVELID::LEVEL_LOBBY);
	CGameInstance::GetInstance()->Reset_Lights();
	CGameInstance::GetInstance()->Delete_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_CAMERA, m_pCamera);

	Safe_Release(m_pCamera);

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

HRESULT CLevel_Lobby::Ready_Layer_BackGround(const LAYER_TYPE eLayerType)
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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
