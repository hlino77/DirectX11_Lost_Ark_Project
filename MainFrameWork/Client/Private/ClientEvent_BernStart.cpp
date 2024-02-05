#include "stdafx.h"
#include "ClientEvent_BernStart.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"
#include "Player.h"
#include "Camera_Player.h"
#include "Camera_Free.h"

CClientEvent_BernStart::CClientEvent_BernStart(_uint iID, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CClientEvent(iID, pDevice, pContext)
{
}

HRESULT CClientEvent_BernStart::Initialize()
{

	return S_OK;
}

void CClientEvent_BernStart::Enter_Event()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);




	Safe_Release(pGameInstance);



}

void CClientEvent_BernStart::Exit_Event()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);



	Safe_Release(pGameInstance);
}

void CClientEvent_BernStart::Tick(_float fTimeDelta)
{
	if (m_iState != (_uint)EVENTSTATE::EVENT)
		return;

	if (fTimeDelta > 1.0f)
		return;


}

void CClientEvent_BernStart::LateTick(_float fTimeDelta)
{

}


HRESULT CClientEvent_BernStart::Render()
{
	return S_OK;
}

void CClientEvent_BernStart::Free()
{
	__super::Free();
}
