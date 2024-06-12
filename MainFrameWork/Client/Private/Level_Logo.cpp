#include "stdafx.h"
#include "..\Public\Level_Logo.h"
#include "Level_Lobby.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "UI.h"
#include "Sound_Manager.h"

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


	if (FAILED(Ready_SoundTrack()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Logo::Tick(_float fTimeDelta)
{


	if (KEY_TAP(KEY::ENTER))
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_SERVERSELECT, L"None"))))
			return E_FAIL;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::E))
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_TOOL, L"None"))))
			return E_FAIL;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::N))
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_TOOL_NPC, L"None"))))
			return E_FAIL;
	}
	else if (KEY_HOLD(KEY::CTRL) && KEY_TAP(KEY::S))
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_STUDIO, L"None"))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLevel_Logo::LateTick(_float fTimeDelta)
{
	
	return S_OK;
}

HRESULT CLevel_Logo::Exit()
{
	CSound_Manager::GetInstance()->Stop_SoundAll();

	return S_OK;
}

HRESULT CLevel_Logo::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CLevel_Logo::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);


	if (nullptr == pGameInstance->Add_GameObject(LEVEL_LOGO, _uint(LAYER_TYPE::LAYER_BACKGROUND), TEXT("Prototype_GameObject_BackGround_LostArk_MainLogo")))
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
