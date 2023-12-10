#include "stdafx.h"
#include "..\Public\Level_Logo.h"

#include "GameInstance.h"
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "UI.h"

CLevel_Logo::CLevel_Logo(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;


	m_bConnect = false;


	m_eState = LOGOSTATE::LOGO;


	if (FAILED(Ready_SoundTrack()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{
	switch (m_eState)
	{
	case LOGOSTATE::LOGO:
		Tick_LOGO(fTimeDelta);
		break;
	case LOGOSTATE::ENTERGAME:
		Tick_ENTERGAME(fTimeDelta);
		break;
	}

	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float fTimeDelta)
{
	


	return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();


	return S_OK;
}

HRESULT CLevel_Logo::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

void CLevel_Logo::Tick_LOGO(_float fTimeDelta)
{
	_bool bKey = false;

	if (KEY_TAP(KEY::ENTER))
	{
		m_eState = LOGOSTATE::ENTERGAME;
	}
}


void CLevel_Logo::Tick_ENTERGAME(_float fTimeDelta)
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
}

HRESULT CLevel_Logo::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance->Add_GameObject(LEVEL_LOGO, _uint(LAYER_TYPE::LAYER_BACKGROUND), TEXT("Prototype_GameObject_BackGround_MainLogo")))
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_Logo * CLevel_Logo::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_Logo*	pInstance = new CLevel_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Logo::Free()
{
 	__super::Free();


}
