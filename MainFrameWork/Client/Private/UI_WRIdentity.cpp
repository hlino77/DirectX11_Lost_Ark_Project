#include "stdafx.h"
#include "UI_WRIdentity.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_WRIdentity::CUI_WRIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WRIdentity::CUI_WRIdentity(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WRIdentity::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WRIdentity::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_WRIdentity");

	return S_OK;
}


void CUI_WRIdentity::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);

}

void CUI_WRIdentity::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_WRIdentity::Render()
{
	return S_OK;
}

void CUI_WRIdentity::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_WRIdentity::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUIBody = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_Body")));
	if (nullptr != pUIBody)
	{
		m_vecUIParts.push_back(pUIBody);
	}
	CUI* pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_GaugeFrame")));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_Gauge")));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_GaugeShine"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_EyeShine"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_Spark"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_SkillIcon"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_Boomb"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WRIdentity_BindingKey"), pUIBody));
	if (nullptr != pUI)
	{
		m_vecUIParts.push_back(pUI);
	}
	Safe_Release(pGameInstance);

	Load_UIData(TEXT("WRIdentity_Gauge_Desc"));

	return S_OK;
}

CUI_WRIdentity* CUI_WRIdentity::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WRIdentity* pInstance = new CUI_WRIdentity(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WRIdentity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WRIdentity::Clone(void* pArg)
{
	CUI_WRIdentity* pInstance = new CUI_WRIdentity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WRIdentity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WRIdentity::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
