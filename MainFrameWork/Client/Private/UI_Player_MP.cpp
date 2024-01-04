#include "stdafx.h"
#include "UI_Player_MP.h"
#include "GameInstance.h"
#include "UI_Manager.h"

CUI_Player_MP::CUI_Player_MP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
CUI_Player_MP::CUI_Player_MP(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_Player_MP::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_Player_MP::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_PlayerMp");

	return S_OK;
}

void CUI_Player_MP::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_Player_MP::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Player_MP::Render()
{
	return S_OK;
}

void CUI_Player_MP::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_Player_MP::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = nullptr;
	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_MPEmpty")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_MPFill")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_MPFrame")));
	if (nullptr == pUI)
		return E_FAIL;
	else
	{
		m_vecUIParts.push_back(pUI);
	}

	Load_UIData(TEXT("PlayerMP_UIDesc"));
	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(0.2f);
	}
	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_Player_MP* CUI_Player_MP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_Player_MP* pInstance = new CUI_Player_MP(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_Player_MP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_Player_MP::Clone(void* pArg)
{
	CUI_Player_MP* pInstance = new CUI_Player_MP(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_Player_MP");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_Player_MP::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
