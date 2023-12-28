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
#include "Camera_Free.h"

CLevel_Lobby::CLevel_Lobby(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Lobby::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	/*if (FAILED(Load_MapData_Lobby(L"../Bin/Resources/MapData/Character_Select_Lobby_ver2.data")))
		return E_FAIL;*/

	if (FAILED(Ready_CameraFree(LAYER_TYPE::LAYER_CAMERA)))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;

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

	return S_OK;
}

HRESULT CLevel_Lobby::LateTick(const _float& fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Lobby::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();

	return S_OK;
}


HRESULT CLevel_Lobby::Ready_Layer_BackGround()
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

	CCamera_Free::CAMERADESC CameraDesc;

	CameraDesc.iLayer = (_uint)eLayerType;
	CameraDesc.vEye = Vec4(0.f, 10.f, -10.f, 1.f);
	CameraDesc.vAt = Vec4(0.f, 0.f, 0.f, 1.f);
	CameraDesc.fFovy = XMConvertToRadians(60.0f);
	CameraDesc.fAspect = (_float)g_iWinSizeX / g_iWinSizeY;
	CameraDesc.fNear = 0.2f;
	CameraDesc.fFar = 1200.0f;

	CameraDesc.TransformDesc.fSpeedPerSec = 5.f;
	CameraDesc.TransformDesc.fRotationPerSec = XMConvertToRadians(90.0f);

	CGameObject* pCamera = pGameInstance->Add_GameObject(LEVEL_LOBBY, _uint(eLayerType), TEXT("Prototype_GameObject_Camera"), &CameraDesc);
	if (pCamera == nullptr)
		return E_FAIL;

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
