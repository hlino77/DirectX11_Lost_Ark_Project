#include "stdafx.h"
#include "UI_WDRIdentity.h"
#include "GameInstance.h"
#include "UI_WDRIdentity_Chain.h"

CUI_WDRIdentity::CUI_WDRIdentity(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_WDRIdentity::CUI_WDRIdentity(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_WDRIdentity::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	if(FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityWDR_UI");

	return S_OK;
}

void CUI_WDRIdentity::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_WDRIdentity::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_WDRIdentity::Render()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity::Ready_Components()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity::Bind_ShaderResources()
{
	return S_OK;
}

HRESULT CUI_WDRIdentity::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_BackGround")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_GaugeFrame")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	CUI* pUIGauge = nullptr;
	pUIGauge = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_Gauge")));
	if (nullptr == pUIGauge)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUIGauge);
	}

	CUI* pUIHammer = nullptr;
	pUIHammer = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_Hammer"), pUIHammer));
	if (nullptr == pUIHammer)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUIHammer);
	}

	CUI_WDRIdentity_Chain::CHAIN_DESC	pChainDesc;
	pChainDesc.pGaugeUI = pUIGauge;
	pChainDesc.pHammerUI = pUIHammer;
	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_Chain"), &pChainDesc));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentityy_BubblesFrame")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	for (size_t i = 0; i < 3; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_Bubble"),&i));
		if (nullptr == pUI)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_WDRIdentity_Skill")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	Load_UIData(TEXT("Identity_Dstroyer_Desc"));

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(0.2f);
	}

	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_WDRIdentity* CUI_WDRIdentity::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_WDRIdentity* pInstance = new CUI_WDRIdentity(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_WDRIdentity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_WDRIdentity::Clone(void* pArg)
{
	CUI_WDRIdentity* pInstance = new CUI_WDRIdentity(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_WDRIdentity");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_WDRIdentity::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}


