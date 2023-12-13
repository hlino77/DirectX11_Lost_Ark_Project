#include "stdafx.h"
#include "..\Public\Loader.h"
#include "GameInstance.h"
#include "AsFileUtils.h"
#include "AsUtils.h"

/* 플레이어 */
#include "Player_Gunslinger.h"
#include "Weapon_Hand.h"
#include "Weapon_Hand_2.h"
#include "Weapon_Long.h"
#include "Weapon_Shot.h"

#include "Camera_Free.h"
#include "StaticModel.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "Camera_Player.h"
#include "BackGround_Server.h"
#include "UI_ServerWnd.h"
#include "UI_ServerGrid.h"
#include "UI_ServerLogo.h"
#include "UI_ServerEntranceButton.h"
#include "UI_Server.h"
#include "UI_Loading.h"
#include "UI_LoadingFill.h"
#include "Monster_Zombie.h"
#include "Monster_Plant.h"


CLoader::CLoader(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: m_pDevice(pDevice)
	, m_pContext(pContext)
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

// unsigned(__stdcall* _beginthreadex_proc_type)(void*);

_uint APIENTRY ThreadEntry(void* pArg)
{
	
	CoInitializeEx(nullptr, 0);

	/* 새롭게 생성된 스레드가 일한다. */
	CLoader*		pLoader = (CLoader*)pArg;

	pLoader->Loading();

	return 0;
}

HRESULT CLoader::Initialize(LEVELID eNextLevel)
{
	InitializeCriticalSection(&m_Critical_Section);

	m_eNextLevel = eNextLevel;

	/* 새로운 스레드를 만들자. */
	/* 스레드 : 내 코드를 읽어주는 하나의 흐름? */
	/* 3 : 생성한 스레드가 호출해야하는 함수의 주소 */
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, ThreadEntry, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

_int CLoader::Loading()
{
	EnterCriticalSection(&m_Critical_Section);

	HRESULT		hr = 0;

	switch (m_eNextLevel)
	{
	case LEVEL_TOOL:
		hr = Loading_For_Level_Tool();
		break;
	case LEVEL_LOGO:
		hr = Loading_For_Level_Logo();
		break;
	case LEVEL_SERVERSELECT:
		hr = Loading_For_Level_ServerSelect();
		break;
	case LEVEL_LOBBY:
		hr = Loading_For_Level_Lobby();
		break;
	case LEVEL_BERN:
		hr = Loading_For_Level_Bern();
		break;
	}

	if (FAILED(hr))
		return -1;	

	LeaveCriticalSection(&m_Critical_Section);

	return 0;	
}

HRESULT CLoader::Loading_For_Level_Logo()
{
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");



	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Tool()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");



	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_ServerSelect()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");
	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_BackGround"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/BackGround/Server_BackGround/%d.jpg", 149))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_Wnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_SelectWnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_WaitingWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_Select_Waiting_Wnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_Grid"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_Select_Grid.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Logo_Grid"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Logo_Grid.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Select_GridWnd"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Server_Select/Server_GridWnd.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Exit"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Exit%d.png",4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Option"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Option%d.png", 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_Button_Return"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Return%d.png", 4))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_JusinLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Logo/Jusin.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_LostArkLogo"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Logo/LostArk_Logo.png"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(LEVEL_SERVERSELECT, TEXT("Prototype_Component_Texture_Server_EntranceButton"),
		CTexture::Create(m_pDevice, m_pContext, L"../Bin/Resources/Textures/UI/Button_UI/Button%d.png",2))))
		return E_FAIL;

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");
	//BackGround
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_BackGround_Server"),
		CBackGround_Server::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	//UI
	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerWnd"),
		CUI_ServerWnd::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerGrid"),
		CUI_ServerGrid::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerLogo"),
		CUI_ServerLogo::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerEntranceButton"),
		CUI_ServerEntranceButton::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_ServerUI"),
		CUI_Server::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_LoadingUI"),
		CUI_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Lobby()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	/* For.GameObject */
	m_strLoading = TEXT("객체원형을 로딩 중 입니다.");



	Safe_Release(pGameInstance);

	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	return S_OK;
}

HRESULT CLoader::Loading_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUI_LoadingFill::m_fLoadingSizeX = 77.f;

	/* For.Texture */
	m_strLoading = TEXT("텍스쳐를 로딩 중 입니다.");

	/* For.Mesh */
	m_strLoading = TEXT("메시를 로딩 중 입니다.");

	/* For.Shader */
	m_strLoading = TEXT("셰이더를 로딩 중 입니다.");

	m_strLoading = TEXT("모델을 로딩 중 입니다.");

	Loading_Model_For_Level_Bern();

	m_strLoading = TEXT("객체 원형을 로딩 중 입니다.");

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Camera_Player"),
		CCamera_Player::Create(m_pDevice, m_pContext, L"Player_Camera"))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Player_Gunslinger"),
		CPlayer_Gunslinger::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Zombie"),
		CMonster_Zombie::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_Monster_Plant"),
		CMonster_Plant::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand"),
		CWeapon_Hand::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Hand_2"),
		CWeapon_Hand_2::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Long"),
		CWeapon_Long::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_GN_WP_Shot"),
		CWeapon_Shot::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(pGameInstance->Add_Prototype(TEXT("Prototype_GameObject_StaticModel"),
		CStaticModel::Create(m_pDevice, m_pContext, PROP))))
		return E_FAIL;

	//Load_MapData(LEVEL_ARENA, L"../Bin/Resources/MapData/Arena.data");
	//Load_ColMesh(LEVEL_ARENA, L"../Bin/Resources/ColMeshData/Arena.data");


	m_strLoading = TEXT("로딩 끝.");
	m_isFinished = true;

	Safe_Release(pGameInstance);

	return S_OK;
}



HRESULT CLoader::Load_MapData(LEVELID eLevel, const wstring& szFilePath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));


	_uint iSize = file->Read<_uint>();

	for (_uint i = 0; i < iSize; ++i)
	{
		wstring szModelName = CAsUtils::ToWString(file->Read<string>());
		Matrix	matWorld = file->Read<Matrix>();

		wstring szModelPath = L"../Bin/Resources/Meshes/Static/";

		wstring strComponentName = L"Prototype_Component_Model_" + szModelName;

		if (FAILED(pGameInstance->Check_Prototype(eLevel, strComponentName)))
		{
			_uint iColCount = file->Read<_uint>();
			for (_uint i = 0; i < iColCount; ++i)
			{
				Vec3 vOffset = file->Read<Vec3>();
				_float fRadius = file->Read<_float>();


				if (file->Read<_bool>())
				{
					Vec3 vOffset = file->Read<Vec3>();
					Vec3 vScale = file->Read<Vec3>();
					Quaternion vQuat = file->Read<Quaternion>();
				}
			}
			continue;
		}
			


		if (FAILED(pGameInstance->Add_Prototype(eLevel, strComponentName,
			CModel::Create(m_pDevice, m_pContext, szModelPath, szModelName, true, false, PivotMatrix))))
			return E_FAIL;


		_uint iColCount = file->Read<_uint>();

		for (_uint i = 0; i < iColCount; ++i)
		{
			Vec3 vOffset = file->Read<Vec3>();
			_float fRadius = file->Read<_float>();


			if (file->Read<_bool>())
			{
				Vec3 vOffset = file->Read<Vec3>();
				Vec3 vScale = file->Read<Vec3>();
				Quaternion vQuat = file->Read<Quaternion>();
			}
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Load_ColMesh(LEVELID eLevel, const wstring& szFilePath)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	shared_ptr<CAsFileUtils> file = make_shared<CAsFileUtils>();
	file->Open(szFilePath, FileMode::Read);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));


	_uint iSize = file->Read<_uint>();

	for (_uint i = 0; i < iSize; ++i)
	{
		wstring szModelName = CAsUtils::ToWString(file->Read<string>());
		Matrix	matWorld = file->Read<Matrix>();

		wstring szModelPath = L"../Bin/Resources/Meshes/Static/";

		wstring strComponentName = L"Prototype_Component_Model_" + szModelName;

		if (FAILED(pGameInstance->Check_Prototype(eLevel, strComponentName)))
		{
			_uint iColCount = file->Read<_uint>();
			for (_uint i = 0; i < iColCount; ++i)
			{
				Vec3 vOffset = file->Read<Vec3>();
				_float fRadius = file->Read<_float>();


				if (file->Read<_bool>())
				{
					Vec3 vOffset = file->Read<Vec3>();
					Vec3 vScale = file->Read<Vec3>();
					Quaternion vQuat = file->Read<Quaternion>();
				}
			}
			continue;
		}


		if (FAILED(pGameInstance->Add_Prototype(eLevel, strComponentName,
			CModel::Create(m_pDevice, m_pContext, szModelPath, szModelName, true, true, PivotMatrix))))
			continue;


		_uint iColCount = file->Read<_uint>();

		for (_uint i = 0; i < iColCount; ++i)
		{
			Vec3 vOffset = file->Read<Vec3>();
			_float fRadius = file->Read<_float>();


			if (file->Read<_bool>())
			{
				Vec3 vOffset = file->Read<Vec3>();
				Vec3 vScale = file->Read<Vec3>();
				Quaternion vQuat = file->Read<Quaternion>();
			}
		}
	}

	Safe_Release(pGameInstance);
	return S_OK;
}



HRESULT CLoader::Loading_Model_For_Level_Bern()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));



	{
		wstring strFileName = L"Gunslinger";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 154.f;
	}

	{
		wstring strFileName = L"GN_Face";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 231.f;
	}

	{
		wstring strFileName = L"GN_Legend_Helmet";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 308.f;
	}

	{
		wstring strFileName = L"GN_Legend_Body";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 385.f;
	}

	{
		wstring strFileName = L"GN_Legend_Leg";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 462.f;
	}

	{
		wstring strFileName = L"GN_Mococo_Head";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 539.f;
	}

	{
		wstring strFileName = L"GN_Mococo_Body";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 616.f;
	}

	{
		wstring strFileName = L"GN_WP_Hand_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 693.f;
	}

	{
		wstring strFileName = L"GN_WP_Long_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 800.f;
	}

	{
		wstring strFileName = L"GN_WP_Shot_Legend";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 1000.f;
	}


	{
		wstring strFileName = L"SM_ENV_TCHEXA_ArenaGround_Aa";
		wstring strFilePath = L"../Bin/Resources/Meshes/Static/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_STATIC, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, PivotMatrix))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 1200.f;
	}

	{
		wstring strFileName = L"Monster_Zombie";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
		CUI_LoadingFill::m_fLoadingSizeX = 1500.f;
	}

	{
		wstring strFileName = L"Monster_Plant";
		wstring strFilePath = L"../Bin/Resources/Meshes/";
		wstring strComponentName = L"Prototype_Component_Model_" + strFileName;

		if (FAILED(pGameInstance->Add_Prototype(LEVEL_BERN, strComponentName,
			CModel::Create(m_pDevice, m_pContext, strFilePath, strFileName, true, false, XMMatrixRotationY(XMConvertToRadians(270.0f))))))
			return E_FAIL;
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLoader::Loading_Model_For_Level_Lobby()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Matrix		PivotMatrix = XMMatrixIdentity();
	PivotMatrix = XMMatrixRotationY(XMConvertToRadians(180.0f));

	Safe_Release(pGameInstance);
	return S_OK;
}

CLoader * CLoader::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel)
{
	CLoader*	pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel)))
	{
		MSG_BOX("Failed to Created : CLoader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	WaitForSingleObject(m_hThread, INFINITE);	

	DeleteCriticalSection(&m_Critical_Section);

	CloseHandle(m_hThread);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}