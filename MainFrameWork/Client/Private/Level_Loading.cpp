#include "stdafx.h"
#include "..\Public\Level_Loading.h"

#include "GameInstance.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_Bern.h"
#include "Level_Lobby.h"
#include "Level_ServerSelect.h"
#include "Level_Tool.h"
#include "Level_ChaosLevel1.h"
#include "Level_ChaosLevel2.h"
#include "Level_ChaosLevel3.h"

#include "UI_Loading.h"
#include "UI_Manager.h"

CLevel_Loading::CLevel_Loading(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}


HRESULT CLevel_Loading::Initialize(LEVELID eNextLevel, const wstring& szBackGruond)
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CLevel* pLevel = pGameInstance->Get_CurrLevel();
	if (pLevel)
		pLevel->Exit();

	m_eNextLevel = eNextLevel;

	m_szBackGruond = szBackGruond;

	if (szBackGruond != L"None")
	{
 		wstring szProtoName = L"Prototype_GameObject_BackGround_" + szBackGruond;
		if (nullptr == pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_BACKGROUND), szProtoName))
			return E_FAIL;
	
		CGameObject* pLoadingUI = pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_LoadingUI"));
		if (nullptr == pLoadingUI)
			return E_FAIL;
		
		CUI_Manager::GetInstance()->Add_UI(LEVEL_LOADING, static_cast<CUI*>(pLoadingUI));
	
	}

	pGameInstance->Set_Loading(true);
	pGameInstance->Set_LoadingNext(eNextLevel);

	/* m_eNextLevel 에 대한 로딩작업을 수행한다. */
	/* 로딩을 겁나 하고있다. */
	m_pLoader = CLoader::Create(m_pDevice, m_pContext, m_eNextLevel);
	if (nullptr == m_pLoader)
		return E_FAIL;


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_Loading::Tick(const _float& fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_Loading::LateTick(const _float& fTimeDelta)
{
	wstring		strLoadingText = m_pLoader->Get_LoadingText();

	SetWindowText(g_hWnd, strLoadingText.c_str());

	if (true == m_pLoader->Get_Finished())
	{
		CGameInstance* pGameInstance = CGameInstance::GetInstance();
		Safe_AddRef(pGameInstance);

		CLevel* pNewLevel = nullptr;

		switch (m_eNextLevel)
		{
		case LEVEL_TOOL:
			pNewLevel = CLevel_Tool::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LOGO:
			pNewLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_SERVERSELECT:
			pNewLevel = CLevel_ServerSelect::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LOBBY:
			pNewLevel = CLevel_Lobby::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_BERN:
			pNewLevel = CLevel_Bern::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_CHAOS_1:
			pNewLevel = CLevel_ChaosLevel1::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_CHAOS_2:
			pNewLevel = CLevel_ChaosLevel2::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_CHAOS_3:
			pNewLevel = CLevel_ChaosLevel3::Create(m_pDevice, m_pContext);
			break;
		}

		if (nullptr == pNewLevel)
			return E_FAIL;

		if (FAILED(Exit()))
			return E_FAIL;

		if (FAILED(pGameInstance->Open_Level(m_eNextLevel, pNewLevel)))
			return E_FAIL;

		Safe_Release(pGameInstance);
	}


	return S_OK;
}

HRESULT CLevel_Loading::Render_Debug()
{
	
	return S_OK;
}

HRESULT CLevel_Loading::Exit()
{
	CUI_Manager::GetInstance()->Clear(LEVELID::LEVEL_LOADING);

	return S_OK;
}

CLevel_Loading * CLevel_Loading::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext, LEVELID eNextLevel, const wstring& szBackGruond)
{
	CLevel_Loading*	pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevel, szBackGruond)))
	{
		MSG_BOX("Failed to Created : CLevel_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
