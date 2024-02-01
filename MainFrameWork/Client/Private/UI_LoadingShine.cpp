#include "stdafx.h"
#include "UI_LoadingShine.h"
#include "GameInstance.h"
#include "UI_LoadingFill.h"

CUI_LoadingShine::CUI_LoadingShine(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CUI(pDevice, pContext)
{
    m_pDevice = pDevice;
    m_pContext = pContext;

    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

CUI_LoadingShine::CUI_LoadingShine(const CUI& rhs)
    : CUI(rhs)
{
}

HRESULT CUI_LoadingShine::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CUI_LoadingShine::Initialize(void* pArg)
{
    if (FAILED(Ready_Components()))
        return E_FAIL;

    m_strUITag = TEXT("Loading_Shine");

    m_fX = -100.f;
    m_fY = 850.f;
    m_fSizeX = 200.f;
    m_fSizeY = 10.f;
    m_fAlpha = 1.f;

    m_pTransformCom->Set_Scale(Vec3(m_fSizeX, m_fSizeY, 1.f));
    m_pTransformCom->Set_State(CTransform::STATE_POSITION,
        Vec3(m_fX - (g_iWinSizeX * 0.5f) + (m_fSizeX*0.5f), -m_fY + g_iWinSizeY * 0.5f, 0.f));

    XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
    XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(g_iWinSizeX, g_iWinSizeY, 0.f, 1.f));
   
    return S_OK;
}

void CUI_LoadingShine::Tick(_float fTimeDelta)
{
}

void CUI_LoadingShine::LateTick(_float fTimeDelta)
{
    if (m_bActive)
        m_pRendererCom->Add_RenderGroup(CRenderer::RENDER_UI, this);
}

HRESULT CUI_LoadingShine::Render()
{
    if (FAILED(Bind_ShaderResources()))
        return E_FAIL;

    m_pShaderCom->Begin(0);

    m_pVIBufferCom->Render();

    return S_OK;
}

void CUI_LoadingShine::Move_fX(_float fTimeDelta)
{

}

HRESULT CUI_LoadingShine::Ready_Components()
{
    __super::Ready_Components();

    /* Com_Texture*/
    if (FAILED(__super::Add_Component(LEVEL_STATIC, TEXT("Prototype_Component_Texture_Loading_Shine"),
        TEXT("Com_Texture"), (CComponent**)&m_pTextureCom)))
        return E_FAIL;

    return S_OK;
}

HRESULT CUI_LoadingShine::Bind_ShaderResources()
{
    if (FAILED(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_pTransformCom->Get_WorldMatrix())))
        return S_OK;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
        return E_FAIL;
    if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
        return E_FAIL;

    if (FAILED(m_pShaderCom->Bind_RawValue("g_Alpha", &m_fAlpha, sizeof(_float))))
        return E_FAIL;

    m_pTextureCom->Set_SRV(m_pShaderCom, "g_DiffuseTexture");

    return S_OK;
}

CUI_LoadingShine* CUI_LoadingShine::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CUI_LoadingShine* pInstance = new CUI_LoadingShine(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed to Created : CUI_LoadingShine");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CUI_LoadingShine::Clone(void* pArg)
{
    CUI_LoadingShine* pInstance = new CUI_LoadingShine(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed to Cloned : CUI_LoadingShine");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CUI_LoadingShine::Free()
{
    __super::Free();
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
