#include "stdafx.h"
#include "UI_Mouse_Cursor.h"
#include "GameInstance.h"
#include "UI_Mouse_PickedIcon.h"
#include "Player_Skill.h"
#include "Item.h"

CUI_Mouse_Cursor::CUI_Mouse_Cursor(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_Mouse_Cursor::CUI_Mouse_Cursor(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_Mouse_Cursor::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_Mouse_Cursor::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if(FAILED(Initialize_Utility()))
        return E_FAIL;

    m_strUITag = TEXT("Mouse_Cursor");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 850.f;
    m_fSizeX = 60.f;
    m_fSizeY = 60.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    //ShowCursor(false);

    return S_OK;
}

HRESULT CUI_Mouse_Cursor::Initialize_Utility()
{
    m_pUitility_PickedIcon = static_cast<CUI_Mouse_PickedIcon*>(CGameInstance::GetInstance()->
        Add_GameObject(LEVEL_STATIC, _uint(LAYER_TYPE::LAYER_UI), TEXT("Prototype_GameObject_Mouse_PickedIcon")));

    if (nullptr == m_pUitility_PickedIcon)
        return E_FAIL;

    return S_OK;
}

void CUI_Mouse_Cursor::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);

    Moving_MouseCursor();
    

}

void CUI_Mouse_Cursor::LateTick(_float fTimeDelta)
{
    m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_MOUSE, this);
}

HRESULT CUI_Mouse_Cursor::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(2);

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_Mouse_Cursor::Picked_Icon(CTexture* pTexture_Icon, _uint iItemGrade)
{
    if (!m_bPickedIcon)
    {
        m_bPickedIcon = true;
        m_pUitility_PickedIcon->Set_IconTexture(pTexture_Icon, iItemGrade);
    }
}

void CUI_Mouse_Cursor::Reset_Icon()
{
    if (m_bPickedIcon)
    {
         m_bPickedIcon = false;
        m_pUitility_PickedIcon->Set_IconTexture(nullptr, 0);
    }
}

HRESULT CUI_Mouse_Cursor::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Basic_Mouse"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_Mouse_Cursor::Bind_ShaderResources()
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

void CUI_Mouse_Cursor::Moving_MouseCursor()
{
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(g_hWnd, &pt);

    m_fX = pt.x;
    m_fY = pt.y;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));
}

CUI_Mouse_Cursor* CUI_Mouse_Cursor::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_Mouse_Cursor* pInstance = new CUI_Mouse_Cursor(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_Mouse_Cursor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_Mouse_Cursor::Clone(void* pArg)
{
    CUI_Mouse_Cursor* pInstance = new CUI_Mouse_Cursor(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_Mouse_Cursor");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_Mouse_Cursor::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    m_pUitility_PickedIcon->Set_Dead(true);
    Safe_Release(m_pTextureCom);
    Safe_Release(m_pTransformCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pVIBufferCom);
    Safe_Release(m_pRendererCom);
}
