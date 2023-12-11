#include "stdafx.h"
#include "Level_ServerSelect.h"
#include "GameInstance.h"
#include "Level_Loading.h"
#include "ServerSession.h"
#include "Service.h"
#include "ThreadManager.h"
#include "ServerSessionManager.h"
#include "UI.h"

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
	if (KEY_TAP(KEY::ENTER))
	{
		if (FAILED(CGameInstance::GetInstance()->Open_Level(LEVEL_LOADING, CLevel_Loading::Create(m_pDevice, m_pContext, LEVEL_LOBBY, L"MainLogo"))))
			return E_FAIL;
	}

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


	Safe_Release(pGameInstance);

	return S_OK;
}

HRESULT CLevel_ServerSelect::Ready_Layer_UI()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

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
