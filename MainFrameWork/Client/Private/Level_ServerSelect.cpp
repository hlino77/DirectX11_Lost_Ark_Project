#include "stdafx.h"
#include "Level_ServerSelect.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "UI_ServerGrid.h"
#include "UI_ServerEntranceButton.h"

CLevel_ServerSelect::CLevel_ServerSelect(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_ServerSelect::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;


	m_bConnect = false;


	return S_OK;
}

HRESULT CLevel_ServerSelect::Tick(_float fTimeDelta)
{
	return S_OK;
}

HRESULT CLevel_ServerSelect::LateTick(_float fTimeDelta)
{
	
	return S_OK;
}

HRESULT CLevel_ServerSelect::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();


	return S_OK;
}


HRESULT CLevel_ServerSelect::Ready_Layer_BackGround()
{
	/* 원형객체를 복제하여 사본객체를 생성하고 레이어에 추가한다. */
	CGameInstance*		pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_BACKGROUND), TEXT("Prototype_GameObject_BackGround_Server")))
		return E_FAIL;

	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ServerSelect::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	if (nullptr == pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerUI")))
		return E_FAIL;


	if (nullptr == pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerLogo")))
		return E_FAIL;

	CUI_ServerEntranceButton* pServerEntranceButton = static_cast<CUI_ServerEntranceButton*>(pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerEntranceButton")));
	if (nullptr == pServerEntranceButton)
		return E_FAIL;
	else
		pServerEntranceButton->Create_Rect();


	Safe_Release(pGameInstance);

	return S_OK;
}

CLevel_ServerSelect * CLevel_ServerSelect::Create(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
{
	CLevel_ServerSelect*	pInstance = new CLevel_ServerSelect(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CLevel_ServerSelect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_ServerSelect::Free()
{
 	__super::Free();


}
