#include "stdafx.h"
#include "UI_NPC_ChaosDungeon.h"
#include "GameInstance.h"

CUI_NPC_ChaosDungeon::CUI_NPC_ChaosDungeon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_NPC_ChaosDungeon::CUI_NPC_ChaosDungeon(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_NPC_ChaosDungeon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("NPC_UI_ChaosDungeon");

    m_fX = g_iWinSizeX * 0.5f;
    m_fY = 400.f;
    m_fSizeX = 1120.f;
    m_fSizeY = 600.f;
  
    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - g_iWinSizeX * 0.5f, -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));

    return S_OK;
}

void CUI_NPC_ChaosDungeon::Tick(_float fTimeDelta)
{
    __super::Tick(fTimeDelta);
}

void CUI_NPC_ChaosDungeon::LateTick(_float fTimeDelta)
{
    __super::LateTick(fTimeDelta);
}

HRESULT CUI_NPC_ChaosDungeon::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;
    m_pShaderCom->Begin(0);
    m_pVIBufferCom->Render();

    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Npc_ChaosDungeon_Entrance_ChaosDungeon_Wnd"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_NPC_ChaosDungeon::Bind_ShaderResources()
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

CUI_NPC_ChaosDungeon* CUI_NPC_ChaosDungeon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_NPC_ChaosDungeon* pInstance = new CUI_NPC_ChaosDungeon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_NPC_ChaosDungeon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_NPC_ChaosDungeon::Clone(void* pArg)
{
    CUI_NPC_ChaosDungeon* pInstance = new CUI_NPC_ChaosDungeon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_NPC_ChaosDungeon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_NPC_ChaosDungeon::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
