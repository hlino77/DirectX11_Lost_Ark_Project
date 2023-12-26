#include "stdafx.h"
#include "UI_Player_ShieldFill.h"
#include "GameInstance.h"

CUI_Player_ShiedFill::CUI_Player_ShiedFill(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Player_ShiedFill::CUI_Player_ShiedFill(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Player_ShiedFill::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Player_ShiedFill::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Player_ShiedlFill");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 850.f;
    m_fSizeX = g_iWinSizeX;
    m_fSizeY = 10.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
    return S_OK;
}

void CUI_Player_ShiedFill::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_Player_ShiedFill::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_Player_ShiedFill::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CUI_Player_ShiedFill::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Hp_EmptyFrame"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Player_ShiedFill::Bind_ShaderResources()
{
    if (FAILED(__super::Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_Player_ShiedFill* CUI_Player_ShiedFill::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Player_ShiedFill* pInstance = new CUI_Player_ShiedFill(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Player_ShiedFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Player_ShiedFill::Clone(void* pArg)
{
    CUI_Player_ShiedFill* pInstance = new CUI_Player_ShiedFill(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Player_ShiedFill");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Player_ShiedFill::Free()
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
