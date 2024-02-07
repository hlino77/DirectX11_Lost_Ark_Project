#include "stdafx.h"
#include "UI_IdentitySP.h"
#include "GameInstance.h"
#include "ServerSessionManager.h"

CUI_IdentitySP::CUI_IdentitySP(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_IdentitySP::CUI_IdentitySP(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_IdentitySP::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_IdentitySP::Initialize(void* pArg)
{
    if (FAILED(UI_Set()))
        return E_FAIL;

    m_strUITag = TEXT("UI_IdentitySP");

    return S_OK;
}

void CUI_IdentitySP::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_IdentitySP::LateTick(_float fTimeDelta)
{
}

HRESULT CUI_IdentitySP::Render()
{
    return S_OK;
}

HRESULT CUI_IdentitySP::Ready_Components()
{
    return S_OK;
}

HRESULT CUI_IdentitySP::Bind_ShaderResources()
{
    return S_OK;
}

HRESULT CUI_IdentitySP::UI_Set()
{
    CGameInstance* pGameInstance = CGameInstance::GetInstance();
    Safe_AddRef(pGameInstance);

    CUI* pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentitySP_MainWnd")));
    if (nullptr == pUI)
        return E_FAIL;
    else
    {
        m_vecUIParts.push_back(pUI);
    }

    pUI = static_cast<CUI*>(pGameInstance->Add_GameObject(pGameInstance->Get_CurrLevelIndex(), (_uint)LAYER_TYPE::LAYER_UI, TEXT("Prototype_GameObject_IdentitySP_LinkedPlayer"),
        CServerSessionManager::GetInstance()->Get_Player()));
    if (nullptr == pUI)
        return E_FAIL;
    else
    {
        m_vecUIParts.push_back(pUI);
    }


    Safe_Release(pGameInstance);
    return S_OK;
}

CUI_IdentitySP* CUI_IdentitySP::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_IdentitySP* pInstance = new CUI_IdentitySP(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_IdentitySP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_IdentitySP::Clone(void* pArg)
{
    CUI_IdentitySP* pInstance = new CUI_IdentitySP(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_IdentitySP");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_IdentitySP::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
