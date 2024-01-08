#include "stdafx.h"
#include "UI_IdentityMG.h"
#include "GameInstance.h"

CUI_IdentityMG::CUI_IdentityMG(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_IdentityMG::CUI_IdentityMG(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityMG::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityMG::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("IdentityMG_UI");

	return S_OK;
}

void CUI_IdentityMG::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentityMG::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_IdentityMG::Render()
{
	return S_OK;
}

HRESULT CUI_IdentityMG::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_BackGround")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	for (size_t i = 0; i < 7; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_Roap"), &i));
		if (nullptr == pUI)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_Harp")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	for (size_t i = 0; i < 3; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_Gauge"),&i));
		if (nullptr == pUI)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_Skill"), &i));
		if (nullptr == pUI)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityMG_SkillFrame"), &i));
		if (nullptr == pUI)
			return E_FAIL;
		else
		{
			m_vecUIParts.push_back(pUI);
		}
	}

	Load_UIData(TEXT("Identity_Bard_Desc"));
	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(0.2f);
	}



	Safe_Release(pGameInstance);
	return S_OK;
}

CUI_IdentityMG* CUI_IdentityMG::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityMG* pInstance = new CUI_IdentityMG(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityMG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityMG::Clone(void* pArg)
{
	CUI_IdentityMG* pInstance = new CUI_IdentityMG(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityMG");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityMG::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pTextureCom_Frame);
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pRendererCom);
}
