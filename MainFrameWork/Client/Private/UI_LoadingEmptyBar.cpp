#include "stdafx.h"
#include "UI_LoadingEmptyBar.h"
#include "GameInstance.h"

CUI_LoadingEmptyBar::CUI_LoadingEmptyBar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingEmptyBar::CUI_LoadingEmptyBar(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingEmptyBar::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingEmptyBar::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;


    m_strObjectTag = TEXT("Loading_EmptyBar");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 850.f;
    m_fSizeX = g_iWinSizeX;
    m_fSizeY = 10.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
    return S_OK;
}

void CUI_LoadingEmptyBar::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_LoadingEmptyBar::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_LoadingEmptyBar::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CUI_LoadingEmptyBar::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_EmptyBar"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingEmptyBar::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_LoadingEmptyBar* CUI_LoadingEmptyBar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingEmptyBar* pInstance = new CUI_LoadingEmptyBar(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingEmptyBar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingEmptyBar::Clone(void* pArg)
{
    CUI_LoadingEmptyBar* pInstance = new CUI_LoadingEmptyBar(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingEmptyBar");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingEmptyBar::Free()
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
