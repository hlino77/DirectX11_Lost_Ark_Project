#include "stdafx.h"
#include "UI_Player_HP.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Player_HP::CUI_Player_HP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
CUI_Player_HP::CUI_Player_HP(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_HP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Player_HP::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_PlayerHP");

	return S_OK;
}

void CUI_Player_HP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Player_HP::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Player_HP::Render()
{
	return S_OK;
}

void CUI_Player_HP::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Player_HP::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = nullptr;
	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_HPEmpty")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_HPFill")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_HPFrame")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	Load_UIData(TEXT("PlayerHP_UIDesc"));

	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_Player_HP* CUI_Player_HP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Player_HP* pInstance = new CUI_Player_HP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Player_HP::Clone(void* pArg)
{
	CUI_Player_HP* pInstance = new CUI_Player_HP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_HP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Player_HP::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
