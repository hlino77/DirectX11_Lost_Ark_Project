#include "stdafx.h"
#include "UI_ChaosDungeon.h"
#include "GameInstance.h"

CUI_ChaosDungeon::CUI_ChaosDungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

CUI_ChaosDungeon::CUI_ChaosDungeon(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_ChaosDungeon::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("ChaosDungeon_UI");

	return S_OK;
}

void CUI_ChaosDungeon::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_ChaosDungeon::LateTick(_float fTimeDelta)
{

}

HRESULT CUI_ChaosDungeon::Render()
{
	return S_OK;
}

HRESULT CUI_ChaosDungeon::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);
	CUI* pUI = nullptr;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_GaugeFrame")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_GaugeShine")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_GaugeSpinShine")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_Gauge")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_GaugeCut")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_NameFrame")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_ChaosDungeon_TimerFrame")));
	if (nullptr != pUI)
		m_vecUIParts.push_back(pUI);
	else
		return E_FAIL;

	Load_UIData(TEXT("ChaosDungeon_Gauge_Desc"));

	for (auto& iter : m_vecUIParts)
	{
		iter->Set_Z(0.2f);
	}

	Safe_Release(pGameInstance);
	return S_OK;
}

HRESULT CUI_ChaosDungeon::Ready_Components()
{

	return S_OK;
}

HRESULT CUI_ChaosDungeon::Bind_ShaderResources()
{
	return S_OK;
}

CUI_ChaosDungeon* CUI_ChaosDungeon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_ChaosDungeon* pInstance = new CUI_ChaosDungeon(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_ChaosDungeon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_ChaosDungeon::Clone(void* pArg)
{
	CUI_ChaosDungeon* pInstance = new CUI_ChaosDungeon(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_ChaosDungeon");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_ChaosDungeon::Free()
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
