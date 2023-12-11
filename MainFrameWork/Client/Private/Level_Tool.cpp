#include "stdafx.h"
#include "Level_Tool.h"
#include "Level_Lobby.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "UI.h"

CLevel_Tool::CLevel_Tool(ID3D11Device * pDevice, ID3D11DeviceContext * pContext)
	: CLevel(pDevice, pContext)
{
}

HRESULT CLevel_Tool::Initialize()
{
	if (FAILED(Ready_Layer_BackGround()))
		return E_FAIL;

	if (FAILED(Ready_Layer_UI()))
		return E_FAIL;


	if (FAILED(Ready_SoundTrack()))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_Tool::Tick(_float fTimeDelta)
{


	return S_OK;
}

HRESULT CLevel_Tool::LateTick(_float fTimeDelta)
{
	


	return S_OK;
}

HRESULT CLevel_Tool::Exit()
{
	CGameInstance::GetInstance()->StopSoundAll();


	return S_OK;
}

HRESULT CLevel_Tool::Ready_SoundTrack()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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
 	__super::Free();


}
