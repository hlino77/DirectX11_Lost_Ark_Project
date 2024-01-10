#include "stdafx.h"
#include "UI_Lobby.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_Lobby_NickName.h"
#include "UI_Lobby_StageName.h"

CUI_Lobby::CUI_Lobby(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Lobby::CUI_Lobby(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Lobby::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Lobby::Initialize(void* pArg)
{
	m_strUITag = TEXT("UI_Lobby");

	if (FAILED(UI_Set()))
		return E_FAIL;

	return S_OK;
}


void CUI_Lobby::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Lobby::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Lobby::Render()
{
	return S_OK;
}

void CUI_Lobby::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Lobby::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	_uint iLevelIndex = (_uint)pGameInstance->Get_CurrLevelIndex();
	CUI_Lobby_StageName::LOBBYSTAGE_DESC LobbyStageNameDesc;

	for (size_t i = 0; i < 4; i++)
	{
		CUI* pUINickName = static_cast<CUI*>(pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Lobby_NickName"),&i));
		if (nullptr == pUINickName)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUINickName);
			pUINickName->Create_Rect();
		}

		LobbyStageNameDesc.iIndex = i;
		LobbyStageNameDesc.pUI = pUINickName;

		CUI* pUIStageName = static_cast<CUI*>(pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Lobby_StageName"), &LobbyStageNameDesc));
		if (nullptr == pUIStageName)
			return E_FAIL;
		else
			m_vecUIParts.push_back(pUIStageName);
	}

	CUI* pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Lobby_EntranceButton")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(iLevelIndex, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_UI_Lobby_NameChanger")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	//Load_UIData(TEXT("Lobby_UI_Desc"));

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(0.2f);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_Lobby* CUI_Lobby::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Lobby* pInstance = new CUI_Lobby(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Lobby::Clone(void* pArg)
{
	CUI_Lobby* pInstance = new CUI_Lobby(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Lobby");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Lobby::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
