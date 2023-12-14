#include "stdafx.h"
#include "UI_Server.h"
#include "GameInstance.h"
#include "UI_ServerLogo.h"
#include "UI_ServerWnd.h"
#include "UI_ServerEntranceButton.h"
#include "UI_ServerGrid.h"

CUI_Server::CUI_Server(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Server::CUI_Server(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Server::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Server::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strObjectTag = TEXT("UI_Server");

	return S_OK;
}

void CUI_Server::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Server::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Server::Render()
{
	return S_OK;
}

void CUI_Server::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Server::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_ServerLogo* pServerLogo = static_cast<CUI_ServerLogo*>(pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerWnd")));
	if (nullptr == pServerLogo)
		return E_FAIL;
	else
		m_vecServerUI.push_back(pServerLogo);

	CUI_ServerWnd* pServerWnd = static_cast<CUI_ServerWnd*>(pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerLogo")));
	if (nullptr == pServerWnd)
		return E_FAIL;
	else
		m_vecServerUI.push_back(pServerWnd);

	CUI_ServerEntranceButton* pServerEntranceButton = static_cast<CUI_ServerEntranceButton*>(pGameInstance->Add_GameObject(LEVEL_SERVERSELECT, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ServerEntranceButton")));
	if (nullptr == pServerEntranceButton)
		return E_FAIL;
	else
	{
		pServerEntranceButton->Create_Rect();
		m_vecServerUI.push_back(pServerEntranceButton);
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_Server* CUI_Server::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Server* pInstance = new CUI_Server(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Server::Clone(void* pArg)
{
	CUI_Server* pInstance = new CUI_Server(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Server");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Server::Free()
{
	__super::Free();
	for (auto& iter : m_vecServerUI)
	{
		Safe_Release(iter);
	}
	m_vecServerUI.clear();
}

