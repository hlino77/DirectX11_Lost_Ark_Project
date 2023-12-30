#include "stdafx.h"
#include "UI_WRIdentity_BindingKey.h"
#include "GameInstance.h"
#include "UI_WRIdentity_Body.h"
#include "ServerSessionManager.h"
#include "Controller_WR.h"
#include "Player_Slayer.h"

CUI_WRIdentity_BindingKey::CUI_WRIdentity_BindingKey(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_WRIdentity_BindingKey::CUI_WRIdentity_BindingKey(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_WRIdentity_BindingKey::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_WRIdentity_BindingKey::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        m_pBodyUI = static_cast<CUI*>(pArg);
    }

    m_strUITag = TEXT("WRIdentity_BindingKey");

    m_fX = 800.f;
    m_fY = 826.f;
    m_fSizeX = 24.f;
    m_fSizeY = 24.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 0.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 0.1f));

    return S_OK;
}

void CUI_WRIdentity_BindingKey::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    if (nullptr != m_pBodyUI)
    {
        if ((CUI_WRIdentity_Body::WRIDENTITY_TRANSFORM_OFF == static_cast<CUI_WRIdentity_Body*>(m_pBodyUI)->Get_Current_IdentityState())
            ||(CUI_WRIdentity_Body::WRIDENTITY_COOL == static_cast<CUI_WRIdentity_Body*>(m_pBodyUI)->Get_Current_IdentityState()))
            m_bKeyOff = true;
        else
            m_bKeyOff = false;
    }
}

void CUI_WRIdentity_BindingKey::LateTick(_float fTimeDelta)
{
    if(!m_bKeyOff)
        __super::LateTick(fTimeDelta);
}

HRESULT CUI_WRIdentity_BindingKey::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_WRIdentity_BindingKey::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_WR_Identity_Identity_BindingKey"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_WRIdentity_BindingKey::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_WRIdentity_BindingKey* CUI_WRIdentity_BindingKey::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_WRIdentity_BindingKey* pInstance = new CUI_WRIdentity_BindingKey(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_WRIdentity_BindingKey");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_WRIdentity_BindingKey::Clone(void* pArg)
{
    CUI_WRIdentity_BindingKey* pInstance = new CUI_WRIdentity_BindingKey(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_WRIdentity_BindingKey");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_WRIdentity_BindingKey::Free()
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
