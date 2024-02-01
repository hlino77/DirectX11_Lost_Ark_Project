#include "stdafx.h"
#include "UI_ETCIcon_Frame.h"
#include "GameInstance.h"

CUI_ETCIcon_Frame::CUI_ETCIcon_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_ETCIcon_Frame::CUI_ETCIcon_Frame(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_ETCIcon_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_ETCIcon_Frame::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    if (nullptr != pArg)
    {
        _uint* strIndex = static_cast<_uint*>(pArg);
        m_strUITag = TEXT("ETCIcon_Frame");
        m_strUITag += to_wstring(*strIndex);
    }
    else
        m_strUITag = TEXT("ETCIcon_Frame");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = g_iWinSizeY * 0.5f;
    m_fSizeX = 64.f;
    m_fSizeY = 64.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_ETCIcon_Frame::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
    Picking_UI();
   
}

void CUI_ETCIcon_Frame::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
   
    if (m_fCurrCool == m_fCoolMaxTime)
        m_fCoolAngle = XM_PI;
    else
    {
        m_fCoolRatio = 1.0f - (m_fCurrCool / m_fCoolMaxTime);
        m_fCoolAngle = -XM_PI + (2 * XM_PI * m_fCoolRatio);
    }
}

HRESULT CUI_ETCIcon_Frame::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if(m_bHaveItem)
    {
        if (!m_bPicked)
        {
            if (FAILED(Bind_ShaderResources_Test()))
                return E_FAIL;
        }
        else if(m_bPicked)
        {
            if (FAILED(Bind_ShaderResources_Picked()))
                return E_FAIL;
        }
        m_pTextureCom_Item->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
        //m_pPlayer_Skill->Get_Skill_Texture()->Set_SRV(m_pShaderCom, "g_DiffuseTexture", m_iTextureIndex);
        m_pShaderCom->Begin(8);
        m_pVIBufferCom->Render();
    }

    if (FAILED(Bind_ShaderResources_Shine()))
        return E_FAIL;
    m_pTextureCom_Shine->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pTextureCom_Frame->Set_SRV(m_pShaderCom, "g_DiffuseTexture");
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_ETCIcon_Frame::Set_SkillIcon(const wstring& strSkillName)
{
}

HRESULT CUI_ETCIcon_Frame::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ETC_Empty"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

   /* if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Skill_Quick_Step"),
        TEXT("Com_TextureTest"), (CComponent**)&m_pTextureCom_Item)))
        return E_FAIL;*/

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Item_SlotFrame_Shine"),
        TEXT("Com_TextureShine"), (CComponent**)&m_pTextureCom_Shine)))
        return E_FAIL;

    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_ETC_SlotFrame"),
        TEXT("Com_TextureFrame"), (CComponent**)&m_pTextureCom_Frame)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_ETCIcon_Frame::Bind_ShaderResources()
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

HRESULT CUI_ETCIcon_Frame::Bind_ShaderResources_Picked()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vecPickedColor, sizeof(Vec4))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_ETCIcon_Frame::Bind_ShaderResources_Shine()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlphaShine, sizeof(_float))))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Color", &m_vColor, sizeof(Vec4))))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_ETCIcon_Frame::Bind_ShaderResources_Test()
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

    if (FAILED(m_pShaderCom->Bind_RawValue("g_fRatio", &m_fCoolAngle, sizeof(_float))))
        return E_FAIL;

    return S_OK;
}

CUI_ETCIcon_Frame* CUI_ETCIcon_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_ETCIcon_Frame* pInstance = new CUI_ETCIcon_Frame(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_ETCIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_ETCIcon_Frame::Clone(void* pArg)
{
    CUI_ETCIcon_Frame* pInstance = new CUI_ETCIcon_Frame(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_ETCIcon_Frame");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_ETCIcon_Frame::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);

    Safe_Release(m_pTextureCom_Item);//테스트용
    Safe_Release(m_pTextureCom_Shine);//마우스 올릴시 빛나는 이펙트
    Safe_Release(m_pTextureCom_Frame);//프레임
}
