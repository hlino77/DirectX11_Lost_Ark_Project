#include "stdafx.h"
#include "UI_Mouse_ToolTip.h"
#include "GameInstance.h"
#include "Player_Skill.h"

CUI_Mouse_ToolTip::CUI_Mouse_ToolTip(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Mouse_ToolTip::CUI_Mouse_ToolTip(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Mouse_ToolTip::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Mouse_ToolTip::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Mouse_ToolTip");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 44.f;
    m_fSizeY = 44.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_Mouse_ToolTip::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

}

void CUI_Mouse_ToolTip::LateTick(_float fTimeDelta)
{
    if(m_bPicked)
        __super::LateTick(fTimeDelta);
}

HRESULT CUI_Mouse_ToolTip::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_Mouse_ToolTip::Set_IconTexture(CTexture* pTexture)
{
    m_pTextureCom = static_cast<CTexture*>(pTexture->Clone(nullptr, nullptr));
}

HRESULT CUI_Mouse_ToolTip::Ready_Components()
{
    __super::Ready_Components();

    return S_OK;
}

HRESULT CUI_Mouse_ToolTip::Bind_ShaderResources()
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

    return S_OK;
}

CUI_Mouse_ToolTip* CUI_Mouse_ToolTip::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Mouse_ToolTip* pInstance = new CUI_Mouse_ToolTip(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Mouse_ToolTip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Mouse_ToolTip::Clone(void* pArg)
{
    CUI_Mouse_ToolTip* pInstance = new CUI_Mouse_ToolTip(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Mouse_ToolTip");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Mouse_ToolTip::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
