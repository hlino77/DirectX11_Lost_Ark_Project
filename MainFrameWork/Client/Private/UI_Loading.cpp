#include "stdafx.h"
#include "UI_Loading.h"
#include "GameInstance.h"
#include "UI_LoadingLabelTop.h"
#include "UI_LoadingLabelBottom.h"
#include "UI_LoadingEmptyBar.h"
#include "UI_LoadingFill.h"
#include "UI_LoadingArrow.h"
#include "UI_LoadingShine.h"

CUI_Loading::CUI_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Loading::CUI_Loading(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Loading::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Loading::Initialize(void* pArg)
{
    if (FAILED(UI_Set()))
        return E_FAIL;

    m_strUITag = TEXT("UI_Loading");

    return S_OK;
}

void CUI_Loading::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Loading::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_Loading::Render()
{
    return S_OK;
}

void CUI_Loading::UI_Tick(_float fTimeDelta)
{
}

void CUI_Loading::Change_LoadingPer(_float fSizeX)
{
    for (auto& iter : m_vecUIParts)
    {
        if (TEXT("Loading_Fill") == iter->Get_UITag())
            static_cast<CUI_LoadingFill*>(iter)->Change_SizeX(fSizeX);
        if (TEXT("Loading_Arrow") == iter->Get_UITag())
            static_cast<CUI_LoadingArrow*>(iter)->Move_Arrow(fSizeX);
    }
}

HRESULT CUI_Loading::UI_Set()
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    CUI_LoadingLabelTop* pLabelTop = static_cast<CUI_LoadingLabelTop*>(pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_BackGround_LoadingLabel_Top")));
    if (nullptr == pLabelTop)
        return E_FAIL;
    else
        m_vecUIParts.push_back(pLabelTop);

    CUI_LoadingLabelBottom* pLabelBottm = static_cast<CUI_LoadingLabelBottom*>(pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_BackGround_LoadingLabel_Bottom")));
    if (nullptr == pLabelBottm)
        return E_FAIL;
    else
        m_vecUIParts.push_back(pLabelBottm);

    CUI_LoadingEmptyBar* pEmptyBar = static_cast<CUI_LoadingEmptyBar*>(pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_BackGround_Loading_EmptyBar")));
    if (nullptr == pEmptyBar)
        return E_FAIL;
    else
        m_vecUIParts.push_back(pEmptyBar);

    CUI_LoadingFill* pFill = static_cast<CUI_LoadingFill*>(pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_BackGround_Loading_Fill")));
    if (nullptr == pFill)
        return E_FAIL;
    else
        m_vecUIParts.push_back(pFill);

    CUI_LoadingArrow* pArrow = static_cast<CUI_LoadingArrow*>(pGameInstance->Add_GameObject(LEVEL_LOADING, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_BackGround_Loading_Arrow")));
    if (nullptr == pArrow)
        return E_FAIL;
    else
        m_vecUIParts.push_back(pArrow);

    Safe_Release(pGameInstance);

    return S_OK;
}

CUI_Loading* CUI_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Loading* pInstance = new CUI_Loading(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Loading");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Loading::Clone(void* pArg)
{
    CUI_Loading* pInstance = new CUI_Loading(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Loading");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Loading::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
