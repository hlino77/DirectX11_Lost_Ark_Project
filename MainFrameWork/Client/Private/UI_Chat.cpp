#include "stdafx.h"
#include "UI_Chat.h"
#include "GameInstance.h"
#include "UI_ChatFrame.h"
#include "UI_ChatWriteFrame.h"
#include "UI_ChatLanguageIcon.h"
#include "UI_ChatSmallerButton.h"
#include "Chat_Manager.h"

CUI_Chat::CUI_Chat(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_Chat::CUI_Chat(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Chat::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Chat::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_Chat");

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Alpha(0.f);
	}

	return S_OK;
}


void CUI_Chat::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
	if(!CChat_Manager::GetInstance()->Is_Chat())
	{
		m_bChatOff = false;
	}
	else
	{
		m_bChatting = true;
		for (auto& iter : m_vecUIParts)
		{
			iter->Set_Alpha( 0.6f );
		}
	}
	if ((!m_bChatOff)&&(m_bChatting))
	{
		m_fDuration -= fTimeDelta;
		if (1.f >= m_fDuration)
		{
			for (auto& iter : m_vecUIParts)
			{
				iter->Decrease_Alpha(fTimeDelta);
			}
		}
		else if (0 >= m_fDuration)
		{
			m_bChatting = false;
			m_fDuration = 3.f;
		}
	}
}

void CUI_Chat::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Chat::Render()
{
	return S_OK;
}

void CUI_Chat::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Chat::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI_ChatFrame* pServerLogo = static_cast<CUI_ChatFrame*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ChatFrame")));
	if (nullptr == pServerLogo)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pServerLogo);

	CUI_ChatWriteFrame* pServerWnd = static_cast<CUI_ChatWriteFrame*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ChatWriteFrame")));
	if (nullptr == pServerWnd)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pServerWnd);

	CUI_ChatLanguageIcon* pChatLanguageIcon = static_cast<CUI_ChatLanguageIcon*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ChatLanguageIcon")));
	if (nullptr == pChatLanguageIcon)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pChatLanguageIcon);

	CUI_ChatSmallerButton* pChatSmaller = static_cast<CUI_ChatSmallerButton*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_ChatSmallerButton")));
	if (nullptr == pChatSmaller)
		return E_FAIL;
	else
	{
		pChatSmaller->Create_Rect();
		m_vecUIParts.push_back(pChatSmaller);
	}

	Load_UIData(TEXT("Chat_Desc"));

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(1.f);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_Chat* CUI_Chat::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Chat* pInstance = new CUI_Chat(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Chat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Chat::Clone(void* pArg)
{
	CUI_Chat* pInstance = new CUI_Chat(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Chat");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Chat::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
