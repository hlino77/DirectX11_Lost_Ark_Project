#include "stdafx.h"
#include "UI_Mouse_PickedIcon.h"
#include "GameInstance.h"
#include "Player_Skill.h"
#include "Item.h"

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

    m_strUITag = TEXT("Picked_Icon");

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
}

void CUI_Mouse_PickedIcon::LateTick(_float fTimeDelta)
{
    if (nullptr == m_pTextureCom)
        return;
        
    if (m_bRender)
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_MOUSE, this);

    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt); 

    m_fX = pt.x;
    m_fY = pt.y;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

}

HRESULT CUI_Mouse_PickedIcon::Render()
{
    if (nullptr == m_pTextureCom)
        return S_OK;

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pTexture_Grade->Set_SRV(m_pShaderCom, "g_DiffuseTexture",m_iTextureIndex);
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(2);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_Mouse_PickedIcon::Set_IconTexture(CTexture* pTextureCom, _uint iTextureIndex)
{
    m_iTextureIndex = iTextureIndex;
    m_pTextureCom = pTextureCom;
}

HRESULT CUI_Mouse_PickedIcon::Ready_Components()
{
    if (FAILED(__super::Ready_Components()))
        return E_FAIL;
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Inventory_Rank"),
        TEXT("Com_Texture"), (CComponent**)&m_pTexture_Grade)))
        return E_FAIL;

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
    m_pTextureCom = nullptr;
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

    Safe_Release(m_pTexture_Grade);
}
