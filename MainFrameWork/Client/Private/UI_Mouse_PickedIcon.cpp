#include "stdafx.h"
#include "UI_Mouse_PickedIcon.h"
#include "GameInstance.h"
#include "Player_Skill.h"

CUI_Mouse_PickedIcon::CUI_Mouse_PickedIcon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Mouse_PickedIcon::CUI_Mouse_PickedIcon(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Mouse_PickedIcon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Mouse_PickedIcon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Picked_Frame");

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

void CUI_Mouse_PickedIcon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    Picking_UI();
    
    if (m_bPicked)
    {
        POINT pt;
        GetCursorPos(&pt);
        ScreenToClient(g_hWnd, &pt);

        m_fX = pt.x;
        m_fY = pt.y;

        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
    }
    else
    {
        m_pTransformCom->Set_State(CTransform::STATE_POSITION,
            Vec3(-100.f - g_iWinSizeX * 0.5f, -100.f + g_iWinSizeY * 0.5f, 0.f));
    }

}

void CUI_Mouse_PickedIcon::LateTick(_float fTimeDelta)
{
    if((nullptr != m_pTextureCom)&&(m_bPicked))
        __super::LateTick(fTimeDelta);
}

HRESULT CUI_Mouse_PickedIcon::Render()
{
    if ((nullptr != m_pTextureCom) && (m_bPicked))
        return E_FAIL;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_Mouse_PickedIcon::Set_IconTexture(CTexture* pTexture)
{
    m_pTextureCom = static_cast<CTexture*>(pTexture->Clone(nullptr, nullptr));
}

HRESULT CUI_Mouse_PickedIcon::Ready_Components()
{
    __super::Ready_Components();

    return S_OK;
}

HRESULT CUI_Mouse_PickedIcon::Bind_ShaderResources()
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

void CUI_Mouse_PickedIcon::UnPickedIcon()
{
    Safe_Release(m_pTextureCom);
}

CUI_Mouse_PickedIcon* CUI_Mouse_PickedIcon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Mouse_PickedIcon* pInstance = new CUI_Mouse_PickedIcon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Mouse_PickedIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Mouse_PickedIcon::Clone(void* pArg)
{
    CUI_Mouse_PickedIcon* pInstance = new CUI_Mouse_PickedIcon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Mouse_PickedIcon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Mouse_PickedIcon::Free()
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
