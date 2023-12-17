#include "stdafx.h"
#include "UI_Identity_GN.h"
#include "GameInstance.h"
#include "UI_Manager.h"
#include "UI_IdentityGN_BackGroundWing.h"
#include "UI_IdentityGN_BackGroundFrame.h"
#include "UI_IdentityGN_WF_BG.h"
#include "UI_IdentityGN_WeaponFrame.h"
#include "UI_IdentityGN_IconBG.h"
#include "UI_IdentityGN_WF_Trail.h"
#include "UI_IdentityGN_WpIcon.h"
#include "UI_Identity_WF_Front.h"

CUI_IdentityGN::CUI_IdentityGN(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUI(pDevice, pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;

	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}
CUI_IdentityGN::CUI_IdentityGN(const CUI& rhs)
	: CUI(rhs)
{
}

HRESULT CUI_IdentityGN::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CUI_IdentityGN::Initialize(void* pArg)
{
	if (FAILED(UI_Set()))
		return E_FAIL;

	m_strUITag = TEXT("UI_IdentityGN");

	return S_OK;
}

void CUI_IdentityGN::Tick(_float fTimeDelta)
{
	__super::Tick(fTimeDelta);
}

void CUI_IdentityGN::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_IdentityGN::Render()
{
	return S_OK;
}

void CUI_IdentityGN::UI_Tick(_float fTimeDelta)
{
}

HRESULT CUI_IdentityGN::UI_Set()
{
	CGameInstance* pGameInstance = CGameInstance::GetInstance();
	Safe_AddRef(pGameInstance);

	CUI* pUI = nullptr;
	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_BackGroundWing")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_BackGround")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WF_BackGround")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WeaponFrame")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WF_IconBG")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WF_Trail")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WF_Icon")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	pUI = static_cast<CUI*>(CGameInstance::GetInstance()->Add_GameObject(LEVEL_STATIC, (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentityGN_WF_Front")));
	if (nullptr == pUI)
		return E_FAIL;
	else
		m_vecUIParts.push_back(pUI);

	Safe_Release(pGameInstance);

	return S_OK;
}

CUI_IdentityGN* CUI_IdentityGN::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CUI_IdentityGN* pInstance = new CUI_IdentityGN(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CUI_IdentityGN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CUI_IdentityGN::Clone(void* pArg)
{
	CUI_IdentityGN* pInstance = new CUI_IdentityGN(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CUI_IdentityGN");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUI_IdentityGN::Free()
{
	__super::Free();
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

	for (auto& iter : m_vecUIParts)
	{
		Safe_Release(iter);
	}
	m_vecUIParts.clear();
}
